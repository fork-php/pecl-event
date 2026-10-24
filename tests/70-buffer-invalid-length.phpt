--TEST--
Check for EventBuffer::read, copyout and pullup with out-of-range lengths
--SKIPIF--
<?php
if (PHP_VERSION_ID < 80000) die('skip PHP 8+ only (ValueError)');
?>
--FILE--
<?php
$bufferClass = EVENT_NS . '\\EventBuffer';
$data = null;

$b = new $bufferClass();
$b->add("0123456789");

foreach ([
    fn() => $b->read(-1),
    fn() => $b->copyout($data, -1),
] as $f) {
try {
    $f();
} catch (ValueError $e) {
    echo get_class($e), ': ', $e->getMessage(), "\n";
}
}

// Requesting more than available is clamped to the buffer length
var_dump($b->copyout($data, PHP_INT_MAX), $data);
var_dump($b->read(PHP_INT_MAX));

// pullup() with size less than the buffer length returns the linearized part only
$b->add("0123456789");
$b->add("abcdef");
var_dump($b->pullup(4));
var_dump($b->pullup(-1));
?>
--EXPECTF--
ValueError: %s::read(): Argument #1 ($max_bytes) must be greater than or equal to 0
ValueError: %s::copyout(): Argument #2 ($max_bytes) must be greater than or equal to 0
int(10)
string(10) "0123456789"
string(10) "0123456789"
string(4) "0123"
string(16) "0123456789abcdef"
