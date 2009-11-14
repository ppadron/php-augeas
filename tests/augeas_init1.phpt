--TEST--
augeas_init() basic test
--INI--
open_basedir="/"
--FILE--
<?php

$augeas = augeas_init();

var_dump($augeas);

?>
--EXPECTF--
resource(%d) of type (augeas)
