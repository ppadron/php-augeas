<?php

require_once 'PHPUnit/Framework/TestCase.php';

class AugeasSaveTest extends PHPUnit_Framework_TestCase
{
    public function setUp()
    {
        $this->augeas = new Augeas(dirname(__FILE__) . "/root");
    }

    public function testShouldSaveWhenNothingWasChanged()
    {
        $this->assertEquals(true, $this->augeas->save());
    }

    public function testShouldThrowExceptionIfSaveFailsWithMalformedFile()
    {
        $this->expectedException = 'AugeasException';
        $this->augeas->rm('/files/etc/hosts/1/ipaddr');
        $this->augeas->save();
    }

}
