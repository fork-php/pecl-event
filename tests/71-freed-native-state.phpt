--TEST--
Check for EventBase and Event methods called after free()
--FILE--
<?php
$baseClass  = EVENT_NS . '\\EventBase';
$eventClass = EVENT_NS . '\\Event';

$base = new $baseClass();
$e  = new $eventClass($base, -1, $eventClass::TIMEOUT, function () {});
$e2 = new $eventClass($base, -1, $eventClass::TIMEOUT, function () {});

function try_call($f) {
    try {
        $f();
    } catch (Error $e) {
        echo $e->getMessage(), "\n";
    }
}

$e->free();
try_call(function () use ($base, $e, $e2, $eventClass) { $e->setPriority(0); });
try_call(function () use ($base, $e, $e2, $eventClass) { $e->removeTimer(); });
try_call(function () use ($base, $e, $e2, $eventClass) { $e->set($base, null); });
var_dump($e->pending);

$base->free();
try_call(function () use ($base, $e, $e2, $eventClass) { $base->dispatch(); });
try_call(function () use ($base, $e, $e2, $eventClass) { $base->loop(); });
try_call(function () use ($base, $e, $e2, $eventClass) { $base->stop(); });
try_call(function () use ($base, $e, $e2, $eventClass) { $base->getMethod(); });
// EventBase::set() is not available in PHP 7 builds
if (method_exists($base, 'set')) {
	try_call(function () use ($base, $e2) { $base->set($e2); });
} else {
	echo "EventBase object is malformed or freed\n";
}
try_call(function () use ($base, $e, $e2, $eventClass) { $eventClass::timer($base, function () {}); });
try_call(function () use ($base, $e, $e2, $eventClass) { $e2->set($base, null); });
echo "done\n";
?>
--EXPECT--
Event object is malformed or freed
Event object is malformed or freed
Event object is malformed or freed
NULL
EventBase object is malformed or freed
EventBase object is malformed or freed
EventBase object is malformed or freed
EventBase object is malformed or freed
EventBase object is malformed or freed
EventBase object is malformed or freed
EventBase object is malformed or freed
done
