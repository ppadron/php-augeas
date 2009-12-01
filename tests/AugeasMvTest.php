<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasMvTest extends PHPUnit_Framework_TestCase
{

    public function setUp()
    {
    	$this->augeas = new Augeas(dirname(__FILE__) . "/root");
    }

    public function testShouldMoveNodeWithoutChildren()
    {
        $value = $this->augeas->get("/files/etc/hosts/1/canonical");
        $this->augeas->mv("/files/etc/hosts/1/canonical", "/files/etc/hosts/1/alias");
        $this->assertEquals($value, $this->augeas->get("/files/etc/hosts/1/alias"));
    }

    public function testShouldMoveNodeWithChildren()
    {
        $ipaddr = $this->augeas->get("/files/etc/hosts/1/ipaddr");
        $this->augeas->mv("/files/etc/hosts/1", "/files/etc/hosts/2");
        $this->assertEquals($ipaddr, $this->augeas->get("/files/etc/hosts/2/ipaddr"));
    }

    public function testShouldFailIfSourcePathIsInvalid()
    {
        $this->assertEquals(
            false,
            $this->augeas->mv("OMG TOTALLY INVALID PATH", "/files/etc/hosts/2")
        );
    }

}

?>
