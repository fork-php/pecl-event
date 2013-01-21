/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2013 The PHP Group                                |
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

#include "fe.h"
#include "priv.h"

/* {{{ ARGINFO */
ZEND_BEGIN_ARG_INFO(arginfo_event__void, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_base_config_1, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_base_1, 0, 0, 1)
	ZEND_ARG_INFO(0, base)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_base_priority_init, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, n_priorities)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_base_loop, 0, 0, 1)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_base_loopexit, 0, 0, 1)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_1, 0, 0, 1)
	ZEND_ARG_INFO(0, event)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_new, 0, 0, 4)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, fd)
	ZEND_ARG_INFO(0, what)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_set, 0, 0, 3)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, fd)
	ZEND_ARG_INFO(0, what)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_add, 0, 0, 1)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_config_avoid_method, 0, 0, 2)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_config_require_features, 0, 0, 2)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_INFO(0, feature)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_priority_set, 0, 0, 2)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_INFO(0, priority)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_pending, 0, 0, 2)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evtimer_new, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evtimer_set, 0, 0, 3)
	ZEND_ARG_INFO(0, event)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();


ZEND_BEGIN_ARG_INFO_EX(arginfo_evsignal_new, 0, 0, 3)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, signum)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();


#if LIBEVENT_VERSION_NUMBER >= 0x02010000
ZEND_BEGIN_ARG_INFO_EX(arginfo_event_config_set_max_dispatch_interval, 0, 0, 4)
	ZEND_ARG_INFO(0, cfg)
	ZEND_ARG_INFO(0, max_interval)
	ZEND_ARG_INFO(0, max_callbacks)
	ZEND_ARG_INFO(0, min_priority)
ZEND_END_ARG_INFO();
#endif


ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_1, 0, 0, 1)
	ZEND_ARG_INFO(0, bevent)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent__events, 0, 0, 2)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, events)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_socket_new, 0, 0, 1)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, fd)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_socket_connect, 0, 0, 2)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, addr)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_socket_connect_hostname, 0, 0, 4)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, hostname)
	ZEND_ARG_INFO(0, port)
	ZEND_ARG_INFO(0, family)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_set_callbacks, 0, 0, 4)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, readcb)
	ZEND_ARG_INFO(0, writecb)
	ZEND_ARG_INFO(0, eventcb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_setwatermark, 0, 0, 4)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, events)
	ZEND_ARG_INFO(0, lowmark)
	ZEND_ARG_INFO(0, highmark)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_write, 0, 0, 2)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_write_buffer, 0, 0, 2)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(0, buf)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_read, 0, 0, 3)
	ZEND_ARG_INFO(0, bevent)
	ZEND_ARG_INFO(1, data)
	ZEND_ARG_INFO(0, size)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_pair_new, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, events)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_priority_set, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, priority)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_bufferevent_set_timeouts, 0, 0, 3)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, timeout_read)
	ZEND_ARG_INFO(0, timeout_write)
ZEND_END_ARG_INFO();



ZEND_BEGIN_ARG_INFO_EX(arginfo_evbuffer_1, 0, 0, 1)
	ZEND_ARG_INFO(0, buf)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evbuffer_freeze, 0, 0, 2)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_INFO(0, at_front)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evbuffer_add, 0, 0, 2)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_INFO(0, data) 
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evbuffer_add_buffer, 0, 0, 2)
	ZEND_ARG_INFO(0, outbuf)
	ZEND_ARG_INFO(0, inbuf) 
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evbuffer_remove, 0, 0, 3)
	ZEND_ARG_INFO(0, buf)
	ZEND_ARG_INFO(1, data)
	ZEND_ARG_INFO(0, max_bytes)
ZEND_END_ARG_INFO();


ZEND_BEGIN_ARG_INFO_EX(arginfo_event_socket_1, 0, 0, 1)
	ZEND_ARG_INFO(0, socket)
ZEND_END_ARG_INFO();


ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_base_new, 0, 0, 4)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, address)
	ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_1, 0, 0, 1)
	ZEND_ARG_INFO(0, evcon)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_get_peer, 0, 0, 3)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(1, address)
	ZEND_ARG_INFO(1, port)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_set_local_address, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, address)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_set_local_port, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_set_timeout, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, timeout)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_set_max_size, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, max_size)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_connection_set_retries, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, retries)
ZEND_END_ARG_INFO();


ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_1, 0, 0, 1)
	ZEND_ARG_INFO(0, evhttp)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_accept_socket, 0, 0, 2)
	ZEND_ARG_INFO(0, evhttp)
	ZEND_ARG_INFO(0, socket)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_event_evhttp_bind_socket, 0, 0, 3)
	ZEND_ARG_INFO(0, evhttp)
	ZEND_ARG_INFO(0, address)
	ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO();


/* ARGINFO END }}} */


#if HAVE_EVENT_EXTRA_LIB
/* {{{ ARGINFO for extra API */

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_1, 0, 0, 1)
	ZEND_ARG_INFO(0, dns_base)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_new, 0, 0, 2)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, initialize)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_resolv_conf_parse, 0, 0, 3)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_nameserver_ip_add, 0, 0, 2)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, ip)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_load_hosts, 0, 0, 2)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, hosts)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_search_add, 0, 0, 2)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, domain)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_search_ndots_set, 0, 0, 2)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, ndots)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evdns_base_set_option, 0, 0, 3)
	ZEND_ARG_INFO(0, dns_base)
	ZEND_ARG_INFO(0, option)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO();


ZEND_BEGIN_ARG_INFO_EX(arginfo_evconnlistener_1, 0, 0, 1)
	ZEND_ARG_INFO(0, listener)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evconnlistener_new, 0, 0, 5)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, backlog)
	ZEND_ARG_INFO(0, stream)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evconnlistener_new_bind, 0, 0, 5)
	ZEND_ARG_INFO(0, base)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, backlog)
	ZEND_ARG_INFO(0, addr)
	ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evconnlistener_set_cb, 0, 0, 2)
	ZEND_ARG_INFO(0, listener)
	ZEND_ARG_INFO(0, cb)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_evconnlistener_set_error_cb, 0, 0, 2)
	ZEND_ARG_INFO(0, listener)
	ZEND_ARG_INFO(0, cb)
ZEND_END_ARG_INFO();


/* ARGINFO for extra API END }}} */
#endif


/* {{{ event_functions[] */
const zend_function_entry event_functions[] = {
	PHP_FE(event_new,                   arginfo_event_new)
	PHP_FE(event_free,                  arginfo_event_1)
	PHP_FE(event_set,                   arginfo_event_set)
	PHP_FE(event_add,                   arginfo_event_add)
	PHP_FE(event_del,                   arginfo_event_1)
	PHP_FE(event_priority_set,          arginfo_event_priority_set)
	PHP_FE(event_pending,               arginfo_event_pending)
	PHP_FE(event_reinit,                arginfo_event_base_1)
	PHP_FE(event_get_supported_methods, arginfo_event__void)
#if LIBEVENT_VERSION_NUMBER >= 0x02010200
	PHP_FE(event_remove_timer, arginfo_event_1)
#endif

	PHP_FE(evtimer_new,     arginfo_evtimer_new)
	PHP_FE(evtimer_set,     arginfo_evtimer_set)
	PHP_FE(evtimer_pending, arginfo_event_1)

	PHP_FALIAS(evtimer_add, event_add, arginfo_event_add)
	PHP_FALIAS(evtimer_del, event_del, arginfo_event_1)

	PHP_FE(evsignal_new,     arginfo_evsignal_new)

	PHP_FALIAS(evsignal_add,     event_add,     arginfo_event_add)
	PHP_FALIAS(evsignal_del,     event_del,     arginfo_event_1)
	PHP_FALIAS(evsignal_pending, event_pending, arginfo_event_pending)

	PHP_FE(event_base_new,                 arginfo_event__void)
	PHP_FE(event_base_new_with_config,     arginfo_event_base_config_1)
	PHP_FE(event_base_free,                arginfo_event_base_1)
	PHP_FE(event_base_get_method,          arginfo_event_base_1)
	PHP_FE(event_base_get_features,        arginfo_event_base_1)
	PHP_FE(event_base_priority_init,       arginfo_event_base_priority_init)
	PHP_FE(event_base_loop,                arginfo_event_base_loop)
	PHP_FE(event_base_dispatch,            arginfo_event_base_1)
	PHP_FE(event_base_loopexit,            arginfo_event_base_loopexit)
	PHP_FE(event_base_loopbreak,           arginfo_event_base_1)
	PHP_FE(event_base_got_break,           arginfo_event_base_1)
	PHP_FE(event_base_got_exit,            arginfo_event_base_1)
	PHP_FE(event_base_gettimeofday_cached, arginfo_event_base_1)
#if LIBEVENT_VERSION_NUMBER >= 0x02010100
	PHP_FE(event_base_update_cache_time, arginfo_event_base_1)
#endif

	PHP_FE(event_config_new,              arginfo_event__void)
	PHP_FE(event_config_avoid_method,     arginfo_event_config_avoid_method)
	PHP_FE(event_config_require_features, arginfo_event_config_require_features)
#if LIBEVENT_VERSION_NUMBER >= 0x02010000
	PHP_FE(event_config_set_max_dispatch_interval, arginfo_event_config_set_max_dispatch_interval)
#endif

	PHP_FE(bufferevent_socket_new,              arginfo_bufferevent_socket_new)
	PHP_FE(bufferevent_free,                    arginfo_bufferevent_1)
	PHP_FE(bufferevent_socket_connect,          arginfo_bufferevent_socket_connect)
	PHP_FE(bufferevent_socket_connect_hostname, arginfo_bufferevent_socket_connect_hostname)
	PHP_FE(bufferevent_socket_get_dns_error,    arginfo_bufferevent_1)
	PHP_FE(bufferevent_setcb,                   arginfo_bufferevent_set_callbacks)
	PHP_FE(bufferevent_enable,                  arginfo_bufferevent__events)
	PHP_FE(bufferevent_disable,                 arginfo_bufferevent__events)
	PHP_FE(bufferevent_get_enabled,             arginfo_bufferevent_1)
	PHP_FE(bufferevent_get_input,               arginfo_bufferevent_1)
	PHP_FE(bufferevent_get_output,              arginfo_bufferevent_1)
	PHP_FE(bufferevent_setwatermark,            arginfo_bufferevent_setwatermark)
	PHP_FE(bufferevent_write,                   arginfo_bufferevent_write)
	PHP_FE(bufferevent_write_buffer,            arginfo_bufferevent_write_buffer)
	PHP_FE(bufferevent_read,                    arginfo_bufferevent_read)
	PHP_FE(bufferevent_read_buffer,             arginfo_bufferevent_write_buffer)
	PHP_FE(bufferevent_pair_new,                arginfo_bufferevent_pair_new)
	PHP_FE(bufferevent_priority_set,            arginfo_bufferevent_priority_set)
	PHP_FE(bufferevent_set_timeouts,            arginfo_bufferevent_set_timeouts)

	PHP_FE(evbuffer_new,            arginfo_event__void)
	PHP_FE(evbuffer_free,           arginfo_evbuffer_1)
	PHP_FE(evbuffer_freeze,         arginfo_evbuffer_freeze)
	PHP_FE(evbuffer_unfreeze,       arginfo_evbuffer_freeze)
	PHP_FE(evbuffer_get_length,     arginfo_evbuffer_1)
	PHP_FE(evbuffer_lock,           arginfo_evbuffer_1)
	PHP_FE(evbuffer_unlock,         arginfo_evbuffer_1)
	PHP_FE(evbuffer_enable_locking, arginfo_evbuffer_1)
	PHP_FE(evbuffer_add,            arginfo_evbuffer_add)
	PHP_FE(evbuffer_add_buffer,     arginfo_evbuffer_add_buffer)
	PHP_FE(evbuffer_remove,         arginfo_evbuffer_remove)

	PHP_FE(event_socket_get_last_errno, arginfo_event_socket_1)
	PHP_FE(event_socket_get_last_error, arginfo_event_socket_1)

	/* These aliases are for compatibility with libevent extension */

	PHP_FALIAS(event_timer_new,            evtimer_new,               arginfo_evtimer_new)
	PHP_FALIAS(event_timer_set,            evtimer_set,               arginfo_evtimer_set)
	PHP_FALIAS(event_timer_pending,        evtimer_pending,           arginfo_event_1)
	PHP_FALIAS(event_timer_add,            event_add,                 arginfo_event_add)
	PHP_FALIAS(event_timer_del,            event_del,                 arginfo_event_1)

	PHP_FALIAS(event_buffer_free,          bufferevent_free,          arginfo_bufferevent_1)
	PHP_FALIAS(event_buffer_set_callback,  bufferevent_setcb,         arginfo_bufferevent_set_callbacks)
	PHP_FALIAS(event_buffer_enable,        bufferevent_enable,        arginfo_bufferevent__events)
	PHP_FALIAS(event_buffer_disable,       bufferevent_disable,       arginfo_bufferevent__events)
	PHP_FALIAS(event_buffer_watermark_set, bufferevent_setwatermark,  arginfo_bufferevent_setwatermark)
	PHP_FALIAS(event_buffer_write,         bufferevent_write,         arginfo_bufferevent_write)
	PHP_FALIAS(event_buffer_priority_set,  bufferevent_priority_set,  arginfo_bufferevent_priority_set)
	PHP_FALIAS(event_buffer_timeout_set,   bufferevent_set_timeouts,  arginfo_bufferevent_set_timeouts)


#if HAVE_EVENT_EXTRA_LIB
/* {{{ Extra API */

	PHP_FE(evdns_base_new,               arginfo_evdns_base_new)
	PHP_FE(evdns_base_free,              arginfo_evdns_base_1)
	PHP_FE(evdns_base_resolv_conf_parse, arginfo_evdns_resolv_conf_parse)
	PHP_FE(evdns_base_nameserver_ip_add, arginfo_evdns_base_nameserver_ip_add)
	PHP_FE(evdns_base_load_hosts,        arginfo_evdns_base_load_hosts)
	PHP_FE(evdns_base_search_clear,      arginfo_evdns_base_1)
	PHP_FE(evdns_base_search_add,        arginfo_evdns_base_search_add)
	PHP_FE(evdns_base_search_ndots_set,  arginfo_evdns_base_search_ndots_set)
	PHP_FE(evdns_base_set_option,        arginfo_evdns_base_set_option)
	PHP_FE(evdns_base_count_nameservers, arginfo_evdns_base_1)

	PHP_FE(evconnlistener_new,          arginfo_evconnlistener_new)
	PHP_FE(evconnlistener_new_bind,     arginfo_evconnlistener_new_bind)
	PHP_FE(evconnlistener_free,         arginfo_evconnlistener_1)
	PHP_FE(evconnlistener_enable,       arginfo_evconnlistener_1)
	PHP_FE(evconnlistener_disable,      arginfo_evconnlistener_1)
	PHP_FE(evconnlistener_set_cb,       arginfo_evconnlistener_set_cb)
	PHP_FE(evconnlistener_set_error_cb, arginfo_evconnlistener_set_error_cb)
#if LIBEVENT_VERSION_NUMBER >= 0x02000300
	PHP_FE(evconnlistener_get_base,     arginfo_evconnlistener_1)
#endif


	PHP_FE(evhttp_connection_base_new, arginfo_event_evhttp_connection_base_new)
	PHP_FE(evhttp_connection_free, arginfo_event_evhttp_connection_1)
	PHP_FE(evhttp_connection_get_base, arginfo_event_evhttp_connection_1)
	PHP_FE(evhttp_connection_get_peer, arginfo_event_evhttp_connection_get_peer)
	PHP_FE(evhttp_connection_set_local_address, arginfo_event_evhttp_connection_set_local_address)
	PHP_FE(evhttp_connection_set_local_port, arginfo_event_evhttp_connection_set_local_port)
	PHP_FE(evhttp_connection_set_timeout, arginfo_event_evhttp_connection_set_timeout)
	PHP_FE(evhttp_connection_set_max_headers_size, arginfo_event_evhttp_connection_set_max_size)
	PHP_FE(evhttp_connection_set_max_body_size, arginfo_event_evhttp_connection_set_max_size)
	PHP_FE(evhttp_connection_set_retries, arginfo_event_evhttp_connection_set_retries)

	PHP_FE(evhttp_new, arginfo_event_base_1)
	PHP_FE(evhttp_free, arginfo_event_evhttp_1)
	PHP_FE(evhttp_accept_socket, arginfo_event_evhttp_accept_socket)
	PHP_FE(evhttp_bind_socket, arginfo_event_evhttp_bind_socket)
	
/* Extra API END}}} */
#endif


	PHP_FE_END
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sts=4 sw=4 ts=4
 */
