<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasMatchTest extends PHPUnit_Framework_TestCase
{

    public function setUp()
    {
        $this->augeas = new Augeas(dirname(__FILE__) . "/root");
    }

    public function testShouldReturnEmptyArrayIfNoMatch()
    {
        $this->assertEquals(array(), $this->augeas->match("/files/non_existent_directory"));
    }

    public function testShoulReturnArrayWithMatches()
    {
        $expectedArray = array("/files/etc/hosts");
        $this->assertEquals($expectedArray, $this->augeas->match("/files/etc/*"));
    }

    public function testShouldThrowExceptionIfPathIsInvalid()
    {
        $this->expectedException = "AugeasException";
        $this->augeas->match("OMG TOTALLY INVALID PATH");
    }

}

?>
