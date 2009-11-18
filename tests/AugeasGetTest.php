<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasGetTest extends PHPUnit_Framework_TestCase
{

    public function setUp()
    {
	$this->augeas = new Augeas(dirname(__FILE__) . "/root");
    }

    public function testShouldReturnStringIfFound()
    {
        $this->assertEquals("127.0.0.1", $this->augeas->get("/files/etc/hosts/1/ipaddr"));
        $this->assertEquals("localhost", $this->augeas->get("/files/etc/hosts/1/canonical"));
        $this->assertEquals("localhost.localdomain", $this->augeas->get("/files/etc/hosts/1/alias"));
    }

    public function testShouldReturnNullIfNotFound()
    {
	$this->assertEquals(null, $this->augeas->get("/files/etc/hosts/1/non_existent_property"));
    }

    public function testShouldThrowAugeasExceptionIfPathIsInvalid()
    {
	$this->expectedException = "AugeasException";
	$this->assertEquals(null, $this->augeas->get("OMG TOTALLY INVALID PATH"));
    }

}

?>
