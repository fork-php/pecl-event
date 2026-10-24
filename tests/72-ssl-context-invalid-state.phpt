--TEST--
Check for EventSslContext with non-string options and after failed initialization
--SKIPIF--
<?php
if (!class_exists(EVENT_NS . "\\EventSslContext")) die("skip Event extra functions are disabled");
?>
--FILE--
<?php
$ctxClass = EVENT_NS . '\\EventSslContext';

// Invalid method: constructor fails, the object stays uninitialized
$ctx = @new $ctxClass(-1, []);
var_dump(is_string(print_r($ctx, true)));
var_dump(is_string(var_export(@$ctx->local_cert, true)));

// Non-string certificate options are converted, not read as strings
foreach ([[], 123, null, 1.5] as $v) {
	$ctx = @new $ctxClass($ctxClass::TLS_SERVER_METHOD, [
		$ctxClass::OPT_LOCAL_CERT => $v,
		$ctxClass::OPT_LOCAL_PK   => $v,
	]);
	var_dump($ctx instanceof $ctxClass);
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
