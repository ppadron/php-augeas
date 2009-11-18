<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasMatchTest extends PHPUnit_Framework_TestCase
{

    public function testShouldReturnEmptyArrayIfNoMatch()
    {
	$augeas = new Augeas("root");
	$this->assertEquals(array(), $augeas->match("/files/non_existent_directory"));
    }

    public function testShoulReturnArrayWithMatches()
    {
	$augeas = new Augeas("root");
	$expectedArray = array("/files/etc/hosts");
	$this->assertEquals($expectedArray, $augeas->match("/files/etc/*"));
    }

    public function testShouldThrowExceptionIfPathIsInvalid()
    {
	$augeas = new Augeas("root");
	$this->assertEquals(array(), $augeas->match("OMG TOTALLY INVALID PATH"));
    }

}

?>
