--TEST--
Check for EventHttpRequest::getHost/getUri when the value is absent
--SKIPIF--
<?php
if (!class_exists(EVENT_NS . '\\EventHttpRequest')) {
	die("skip Event extra functions are disabled");
}
?>
--FILE--
<?php
$reqClass = EVENT_NS . '\\EventHttpRequest';

$req = new $reqClass(function () {});
var_dump($req->getHost());
var_dump($req->getUri());
?>
--EXPECT--
NULL
NULL
