--TEST--
Check for issue #3: SEGFAULT after EventHttpRequest requests were made over a connection and freed
--SKIPIF--
<?php
if (!class_exists(EVENT_NS . '\\EventHttpRequest')) die("skip Event extra functions are disabled");
?>
--ENV--
ASAN_OPTIONS=detect_leaks=0:abort_on_error=1
--FILE--
<?php
$eventBaseClass = EVENT_NS . '\\EventBase';
$eventClass = EVENT_NS . '\\Event';
$eventHttpClass = EVENT_NS . '\\EventHttp';
$eventHttpRequestClass = EVENT_NS . '\\EventHttpRequest';
$eventHttpConnectionClass = EVENT_NS . '\\EventHttpConnection';

function _callback_server($req) {
	$req->sendReply(200, "OK");
}

function _callback_client($req) {
	global $replies;
	$replies++;
	// The request is owned by libevent after makeRequest(): freeing it here
	// used to cause a double free (SEGFAULT) when the connection was destroyed.
	$req->free();
}

function _callback_timer() {
	global $timer, $conn, $base, $inc, $eventHttpRequestClass;

	$req = new $eventHttpRequestClass('_callback_client');
	$conn->makeRequest($req, $eventHttpRequestClass::CMD_GET, '/');

	if (++$inc < 30) {
		$timer->addTimer(0.01);
	} else {
		// Leave the last request pending: the connection frees it on shutdown
		$base->exit();
	}
}

$inc = 0;
$replies = 0;
$host = '127.0.0.1';
$port = 20000 + (getmypid() % 20000);

$base = new $eventBaseClass();

$http = new $eventHttpClass($base);
$http->setAllowedMethods($eventHttpRequestClass::CMD_GET);
$http->setDefaultCallback('_callback_server');
$http->bind($host, $port);

$timer = $eventClass::timer($base, '_callback_timer');
$timer->addTimer(0.1);

$conn = new $eventHttpConnectionClass($base, NULL, $host, $port);

$base->loop();

echo "requests: $inc\n";
echo "replied: ", var_export($replies > 0, true), "\n";
?>
--EXPECT--
requests: 30
replied: true
