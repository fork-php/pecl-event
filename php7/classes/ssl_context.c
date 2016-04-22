/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Ruslan Osmanov <osmanov@php.net>                             |
   +----------------------------------------------------------------------+
*/
#include "../src/common.h"
#include "../src/util.h"
#include "../src/priv.h"

#include "ssl_context.h"

#ifndef HAVE_EVENT_OPENSSL_LIB
# error "HAVE_EVENT_OPENSSL_LIB undefined"
#endif

/* {{{ Private */


static inline int get_fingerprint(X509* cert, unsigned char *md, unsigned int *n)/*{{{*/
{
	const EVP_MD *digest;

	if (!(digest = EVP_get_digestbyname("sha1"))) {
		php_error_docref(NULL, E_WARNING, "Failed to get digest by 'sha1'");
		return 0;
	}

	if (!X509_digest(cert, digest, md, n)) {
		php_error_docref(NULL, E_WARNING, "Could not generate signature");
		return 0;
	}

	return 1;
}/*}}}*/


static inline int compare_certificates(X509 *c1, X509 *c2)/*{{{*/
{
	unsigned char md1[EVP_MAX_MD_SIZE], md2[EVP_MAX_MD_SIZE];
	unsigned int n1, n2;

	if (!(get_fingerprint(c1, md1, &n1) && get_fingerprint(c2, md2, &n2))) {
		return -1;
	}

	return memcmp(md1, md2, n1);

}/*}}}*/


/* {{{ verify_callback */
static int verify_callback(int preverify_ok, X509_STORE_CTX *ctx)
{
	SSL                      *ssl;
	X509                     *cur_cert;
	int                       err;
	int                       depth;
	char                      buf[256];
	php_event_ssl_context_t  *ectx;
	zval                     *pzval   = NULL;
	HashTable                *ht;

	ssl  = X509_STORE_CTX_get_ex_data(ctx, SSL_get_ex_data_X509_STORE_CTX_idx());
	ectx = (php_event_ssl_context_t *) SSL_get_ex_data(ssl, php_event_ssl_data_index);

	PHP_EVENT_ASSERT(ectx && ectx->ht);
	ht = ectx->ht;

	cur_cert = X509_STORE_CTX_get_current_cert(ctx);
	err      = X509_STORE_CTX_get_error(ctx);
	depth    = X509_STORE_CTX_get_error_depth(ctx);

	X509_NAME_oneline(X509_get_subject_name(cur_cert), buf, sizeof(buf) / sizeof(buf[0]));

	/* If OPT_ALLOW_SELF_SIGNED is set and is TRUE, preverify_ok = 1 */
	if (!preverify_ok
			&& (err == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT)
			&& (pzval = zend_hash_index_find(ht, PHP_EVENT_OPT_ALLOW_SELF_SIGNED)) != NULL
			&& zend_is_true(pzval)) {
		preverify_ok = 1;
	}

	if (!preverify_ok) {
		php_error_docref(NULL, E_NOTICE, "verify error:%d:%s:depth=%d:%s",
				err, X509_verify_cert_error_string(err), depth, buf);
	}

	/*{{{ Fingerprint check */
	if (ectx->match_fingerprints) {
		if (0 != compare_certificates(ctx->current_cert, SSL_CTX_get0_certificate(ssl->ctx))) {
			php_error_docref(NULL, E_NOTICE, "peer-server certificate mismatch");
			preverify_ok = 0;
		}
	}
	/*}}}*/

	/* Verify depth, if OPT_VERIFY_DEPTH option is set */
	if ((pzval = zend_hash_index_find(ht, PHP_EVENT_OPT_VERIFY_DEPTH)) != NULL) {
		convert_to_long_ex(pzval);

		if (depth > Z_LVAL_P(pzval)) {
			preverify_ok = 0;
			err = X509_V_ERR_CERT_CHAIN_TOO_LONG;
			X509_STORE_CTX_set_error(ctx, err);
		}
	}

	if (!preverify_ok && (err == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT)) {
		X509_NAME_oneline(X509_get_issuer_name(ctx->current_cert), buf, sizeof(buf)/ sizeof(buf[0]));
		php_error_docref(NULL, E_NOTICE, "unable to get issuer certificate, issuer: %s", buf);
	}

	return preverify_ok;
}
/* }}} */

/* {{{ passwd_callback */
static int passwd_callback(char *buf, int num, int verify, void *data)
{
	HashTable *ht  = (HashTable *)data;
	zval      *val = NULL;

	if ((val = zend_hash_index_find(ht, PHP_EVENT_OPT_PASSPHRASE)) != NULL) {
		if (Z_STRLEN_P(val) < num - 1) {
			memcpy(buf, Z_STRVAL_P(val), Z_STRLEN_P(val) + 1);
			return Z_STRLEN_P(val);
		}
	}

	return 0;
}
/* }}} */

/* {{{ set_ca */
static zend_always_inline void set_ca(SSL_CTX *ctx, const char *cafile, const char *capath) {
	if (!SSL_CTX_load_verify_locations(ctx, cafile, capath)) {
		php_error_docref(NULL, E_WARNING,
				"Unable to set verify locations `%s' `%s'",
				cafile, capath);
	}
}
/* }}} */

/* {{{ set_ciphers */
static zend_always_inline void set_ciphers(SSL_CTX *ctx, const char *cipher_list)
{
	if (SSL_CTX_set_cipher_list(ctx, cipher_list) != 1) {
		php_error_docref(NULL, E_WARNING,
				"Failed setting cipher list: `%s'", cipher_list);
	}
}
/* }}} */

/* {{{ _php_event_ssl_ctx_set_private_key */
int _php_event_ssl_ctx_set_private_key(SSL_CTX *ctx, const char *private_key)
{
	if (private_key) {
		char resolved_path_buff_pk[MAXPATHLEN];

		if (VCWD_REALPATH(private_key, resolved_path_buff_pk)) {
			if (SSL_CTX_use_PrivateKey_file(ctx, resolved_path_buff_pk, SSL_FILETYPE_PEM) != 1) {
				php_error_docref(NULL, E_WARNING,
						"Unable to set private key file `%s'",
						resolved_path_buff_pk);
				return -1;
			}

			return 0;
		}
	}

	return -1;
}
/* }}} */

/* {{{ _php_event_ssl_ctx_set_local_cert */
int _php_event_ssl_ctx_set_local_cert(SSL_CTX *ctx, const char *certfile, const char *private_key)
{
	char resolved_path_buff[MAXPATHLEN];

	if (VCWD_REALPATH(certfile, resolved_path_buff)) {
		if (SSL_CTX_use_certificate_chain_file(ctx, resolved_path_buff) != 1) {
			php_error_docref(NULL, E_WARNING,
					"SSL_CTX_use_certificate_chain_file failed, file: `%s'", certfile);
			return -1;
		}

		if (private_key) {
			if (_php_event_ssl_ctx_set_private_key(ctx, private_key)) {
				return -1;
			}
		} else {
			if (SSL_CTX_use_PrivateKey_file(ctx, resolved_path_buff, SSL_FILETYPE_PEM) != 1) {
				php_error_docref(NULL, E_WARNING,
						"Unable to set private key file `%s'",
						resolved_path_buff);
				return -1;
			}
		}
	}

	return 0;
}
/* }}} */

/* {{{ set_ssl_ctx_options */
static inline void set_ssl_ctx_options(php_event_ssl_context_t *ectx)
{
	SSL_CTX     *ctx         = ectx->ctx;
	HashTable   *ht          = ectx->ht;
	zend_string *key;
	zval        *zv;
	zend_ulong   idx;
	zend_bool    got_ciphers = 0;
	char        *cafile      = NULL;
	char        *capath      = NULL;

	ZEND_HASH_FOREACH_KEY_VAL(ht, idx, key, zv) {
		if (key) {
			continue;
		}
		switch (idx) {
			case PHP_EVENT_OPT_LOCAL_CERT:
				{
					zval *zpk;

					zpk = zend_hash_index_find(ht, PHP_EVENT_OPT_LOCAL_PK);

					if (zpk == NULL) {
						_php_event_ssl_ctx_set_local_cert(ctx, Z_STRVAL_P(zv), NULL);
					} else {
						_php_event_ssl_ctx_set_local_cert(ctx, Z_STRVAL_P(zv), Z_STRVAL_P(zpk));
					}
					break;
				}
			case PHP_EVENT_OPT_LOCAL_PK:
				/* Skip. SSL_CTX_use_PrivateKey_file is applied in "local_cert". */
				break;
			case PHP_EVENT_OPT_PASSPHRASE:
				convert_to_string_ex(zv);
				SSL_CTX_set_default_passwd_cb_userdata(ctx, ht);
				SSL_CTX_set_default_passwd_cb(ctx, passwd_callback);
				break;
			case PHP_EVENT_OPT_CA_FILE:
				convert_to_string_ex(zv);
				cafile = Z_STRVAL_P(zv);
				break;
			case PHP_EVENT_OPT_CA_PATH:
				convert_to_string_ex(zv);
				capath = Z_STRVAL_P(zv);
				break;
			case PHP_EVENT_OPT_NO_SSLv2:
				if (zend_is_true(zv)) {
					SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
				} else {
					SSL_CTX_clear_options(ctx, SSL_OP_NO_SSLv2);
				}
				break;
			case PHP_EVENT_OPT_NO_SSLv3:
				if (zend_is_true(zv)) {
					SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv3);
				} else {
					SSL_CTX_clear_options(ctx, SSL_OP_NO_SSLv3);
				}
				break;
			case PHP_EVENT_OPT_NO_TLSv1:
				if (zend_is_true(zv)) {
					SSL_CTX_set_options(ctx, SSL_OP_NO_TLSv1);
				} else {
					SSL_CTX_clear_options(ctx, SSL_OP_NO_TLSv1);
				}
				break;
#ifdef SSL_OP_NO_TLSv1_1
			case PHP_EVENT_OPT_NO_TLSv1_1:
				if (zend_is_true(zv)) {
					SSL_CTX_set_options(ctx, SSL_OP_NO_TLSv1_1);
				} else {
					SSL_CTX_clear_options(ctx, SSL_OP_NO_TLSv1_1);
				}
				break;
#endif
#ifdef SSL_OP_NO_TLSv1_2
			case PHP_EVENT_OPT_NO_TLSv1_2:
				if (zend_is_true(zv)) {
					SSL_CTX_set_options(ctx, SSL_OP_NO_TLSv1_2);
				} else {
					SSL_CTX_clear_options(ctx, SSL_OP_NO_TLSv1_2);
				}
				break;
#endif
			case PHP_EVENT_OPT_CIPHER_SERVER_PREFERENCE:
				if (zend_is_true(zv)) {
					SSL_CTX_set_options(ctx, SSL_OP_CIPHER_SERVER_PREFERENCE);
				} else {
					SSL_CTX_clear_options(ctx, SSL_OP_CIPHER_SERVER_PREFERENCE);
				}
				break;
			case PHP_EVENT_OPT_ALLOW_SELF_SIGNED:
				/* Skip */
				break;
			case PHP_EVENT_OPT_VERIFY_PEER:
				if (zend_is_true(zv)) {
					SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);
				} else {
					SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
				}
				break;
			case PHP_EVENT_OPT_VERIFY_DEPTH:
				convert_to_long_ex(zv);
				SSL_CTX_set_verify_depth(ctx, Z_LVAL_P(zv));
				break;
			case PHP_EVENT_OPT_CIPHERS:
				got_ciphers = 1;
				convert_to_string_ex(zv);
				set_ciphers(ctx, Z_STRVAL_P(zv));
				break;
			case PHP_EVENT_OPT_MATCH_FINGERPRINTS:
				ectx->match_fingerprints = (zend_bool) zend_is_true(zv);
				break;
			default:
				php_error_docref(NULL, E_WARNING, "Unknown option %ld", idx);
		}
	} ZEND_HASH_FOREACH_END();

	if (got_ciphers == 0) {
		set_ciphers(ctx, "DEFAULT");
	}

	if (cafile || capath) {
		set_ca(ctx, cafile, capath);
	}
}
/* }}} */

/* {{{ get_ssl_method */
static zend_always_inline SSL_METHOD *get_ssl_method(zend_long in_method)
{
	SSL_METHOD *method;

	switch (in_method) {
		case PHP_EVENT_SSLv2_CLIENT_METHOD:

#ifdef OPENSSL_NO_SSL2
			php_error_docref(NULL, E_WARNING,
					"SSLv2 support is not compiled into the "
					"OpenSSL library PHP is linked against");
			return NULL;
#else
			method = (SSL_METHOD *) SSLv2_client_method();
			break;
#endif
		case PHP_EVENT_SSLv3_CLIENT_METHOD:
			method = (SSL_METHOD *) SSLv3_client_method();
			break;
		case PHP_EVENT_SSLv23_CLIENT_METHOD:
			method = (SSL_METHOD *) SSLv23_client_method();
			break;
		case PHP_EVENT_TLS_CLIENT_METHOD:
			method = (SSL_METHOD *) TLSv1_client_method();
			break;
		case PHP_EVENT_SSLv2_SERVER_METHOD:
#ifdef OPENSSL_NO_SSL2
			php_error_docref(NULL, E_WARNING,
					"SSLv2 support is not compiled into the "
					"OpenSSL library PHP is linked against");
			return NULL;
#else
			method = (SSL_METHOD *) SSLv2_server_method();
			break;
#endif
		case PHP_EVENT_SSLv3_SERVER_METHOD:
			method = (SSL_METHOD *) SSLv3_server_method();
			break;
		case PHP_EVENT_SSLv23_SERVER_METHOD:
			method = (SSL_METHOD *) SSLv23_server_method();
			break;
		case PHP_EVENT_TLS_SERVER_METHOD:
			method = (SSL_METHOD *) TLSv1_server_method();
			break;
		case PHP_EVENT_TLSv11_CLIENT_METHOD:
#ifdef SSL_OP_NO_TLSv1_1
			method = (SSL_METHOD *) TLSv1_1_client_method();
#else
			php_error_docref(NULL, E_WARNING,
					"TLSv1_1 support is not compiled into the "
					"OpenSSL library PHP is linked against");
			return NULL;
#endif
			break;
		case PHP_EVENT_TLSv11_SERVER_METHOD:
#ifdef SSL_OP_NO_TLSv1_1
			method = (SSL_METHOD *) TLSv1_1_server_method();
#else
			php_error_docref(NULL, E_WARNING,
					"TLSv1_1 support is not compiled into the "
					"OpenSSL library PHP is linked against");
			return NULL;
#endif
			break;
		case PHP_EVENT_TLSv12_CLIENT_METHOD:
#ifdef SSL_OP_NO_TLSv1_2
			method = (SSL_METHOD *) TLSv1_2_client_method();
#else
			php_error_docref(NULL, E_WARNING,
					"TLSv1_2 support is not compiled into the "
					"OpenSSL library PHP is linked against");
			return NULL;
#endif
			break;
		case PHP_EVENT_TLSv12_SERVER_METHOD:
#ifdef SSL_OP_NO_TLSv1_2
			method = (SSL_METHOD *) TLSv1_2_server_method();
#else
			php_error_docref(NULL, E_WARNING,
					"TLSv1_2 support is not compiled into the "
					"OpenSSL library PHP is linked against");
			return NULL;
#endif
			break;
		default:
			return NULL;
	}

	return method;
}
/* }}} */

/* Private }}} */


/* {{{ proto EventSslContext EventSslContext::__construct(int method, array options);
 *
 * Creates SSL context holding pointer to SSL_CTX.
 * method parameter is one of EventSslContext::*_METHOD constants.
 * options parameter is an associative array of SSL context options */
PHP_METHOD(EventSslContext, __construct)
{
	php_event_ssl_context_t *ectx;
	HashTable               *ht_options;
	zend_long                in_method;
	SSL_METHOD              *method;
	SSL_CTX                 *ctx;
	zend_long                options    = SSL_OP_ALL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "lh",
				&in_method, &ht_options) == FAILURE) {
		return;
	}

	method = get_ssl_method(in_method);
	if (method == NULL) {
		php_error_docref(NULL, E_WARNING, "Invalid method passed: %ld", in_method);
		return;
	}

	ctx = SSL_CTX_new(method);
	if (ctx == NULL) {
		php_error_docref(NULL, E_WARNING, "Creation of a new SSL_CTX object failed");
		return;
	}

	ectx = Z_EVENT_SSL_CONTEXT_OBJ_P(getThis());
	ectx->ctx = ctx;

	ALLOC_HASHTABLE(ectx->ht);
	zend_hash_init(ectx->ht, zend_hash_num_elements(ht_options), NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(ectx->ht, ht_options, (copy_ctor_func_t) zval_add_ref);

	SSL_CTX_set_options(ectx->ctx, options);
	set_ssl_ctx_options(ectx);

	/* Issue #20 */
	SSL_CTX_set_session_id_context(ectx->ctx, (unsigned char *)(void *)ectx->ctx, sizeof(ectx->ctx));
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 sts=4 fdm=marker
 * vim<600: noet sw=4 ts=4 sts=4
 */
