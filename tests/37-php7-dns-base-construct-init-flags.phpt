--TEST--
Check for EventDnsBase::__construct parsing of the initialization flags
--SKIPIF--
<?php
if (PHP_VERSION_ID >= 80000 || PHP_VERSION_ID < 70000) {
	die("skip only for PHP 7");
}
if (!class_exists(EVENT_NS . "\\EventDnsBase")) {
	die("skip Event extra functions are disabled");
}

$eventUtilClass = EVENT_NS . '\\EventUtil';
if ($eventUtilClass::LIBEVENT_VERSION_NUMBER < 0x02010a00) {
    die('skip this test is for libevent version >= 2.1');
}
?>
--FILE--
<?php
$eventDnsBaseClass = EVENT_NS . '\\EventDnsBase';
$eventBaseClass = EVENT_NS . '\\EventBase';

$base = new $eventBaseClass();

new $eventDnsBaseClass($base, false);
echo "false: ok\n";

new $eventDnsBaseClass($base, true);
echo "true: ok\n";

new $eventDnsBaseClass($base, $eventDnsBaseClass::INITIALIZE_NAMESERVERS);
echo "INITIALIZE_NAMESERVERS: ok\n";

new $eventDnsBaseClass($base, $eventDnsBaseClass::DISABLE_WHEN_INACTIVE);
echo "DISABLE_WHEN_INACTIVE: ok\n";

new $eventDnsBaseClass($base, $eventDnsBaseClass::NAMESERVERS_NO_DEFAULT);
echo "NAMESERVERS_NO_DEFAULT: ok\n";

try {
    new $eventDnsBaseClass($base, -1);
} catch (\Throwable $e) {
    printf("Invalid flags value exception %s: %s\n", get_class($e), $e->getMessage());
}

try {
    new $eventDnsBaseClass($base, new \stdClass());
} catch (TypeError $e) {
    printf("Invalid flags type exception %s: %s\n", get_class($e), $e->getMessage());
}
?>
--EXPECT--
false: ok
true: ok
INITIALIZE_NAMESERVERS: ok
DISABLE_WHEN_INACTIVE: ok
NAMESERVERS_NO_DEFAULT: ok
Invalid flags value exception EventException: Invalid initialization flags
Invalid flags type exception TypeError: EventDnsBase::__construct(): Argument #2 ($initialize) must be of type int|bool, object given
