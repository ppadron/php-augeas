--TEST--
augeas_init() should fail if access to root directory is not allowed by open_basedir
--INI--
display_errors="off"
open_basedir="/tmp"
--FILE--
<?php

$augeas = augeas_init("/");

var_dump($augeas);

?>
--EXPECT--
bool(false)
