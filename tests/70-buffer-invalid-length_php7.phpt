--TEST--
Check for EventBuffer::read, copyout and pullup with out-of-range lengths (PHP 7)
--SKIPIF--
<?php
if (PHP_VERSION_ID >= 80000) die('skip PHP 7 only (warnings instead of ValueError)');
?>
--FILE--
<?php
$bufferClass = EVENT_NS . '\\EventBuffer';

$b = new $bufferClass();
$b->add("0123456789");

var_dump(@$b->read(-1));
var_dump(@$b->copyout($data, -1));

// Requesting more than available is clamped to the buffer length
var_dump($b->copyout($data, PHP_INT_MAX), $data);
var_dump($b->read(PHP_INT_MAX));

// pullup() with size less than the buffer length returns the linearized part only
$b->add("0123456789");
$b->add("abcdef");
var_dump($b->pullup(4));
var_dump($b->pullup(-1));
?>
--EXPECT--
bool(false)
int(-1)
int(10)
string(10) "0123456789"
string(10) "0123456789"
string(4) "0123"
string(16) "0123456789abcdef"
