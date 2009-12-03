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
        $expectedArray = array("/files/etc/hosts" => null);
        $this->assertEquals($expectedArray, $this->augeas->match("/files/etc/*"));
    }

    public function testShouldReturnArrayWithNodesAndValues()
    {
        $expectedArray = array(
            "/files/etc/hosts/1/ipaddr"    => "127.0.0.1",
            "/files/etc/hosts/1/canonical" => "localhost",
            "/files/etc/hosts/1/alias"     => "localhost.localdomain",
        );

        $matches = $this->augeas->match("/files/etc/hosts/1/*");

        $this->assertEquals($expectedArray, $matches);
    }

    public function testShouldThrowExceptionIfPathIsInvalid()
    {
        $this->expectedException = "AugeasException";
        $this->augeas->match("OMG TOTALLY INVALID PATH");
    }

}

?>
