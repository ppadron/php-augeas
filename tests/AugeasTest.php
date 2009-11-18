<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasTest extends PHPUnit_Framework_TestCase
{

    public function testShouldSetAugeasRoot()
    {
        // trailling slashes are added here to correctly compare with /augeas/root
        $root = "/tmp/";
        $augeas = new Augeas("/tmp/");
        $this->assertEquals($root, $augeas->get("/augeas/root"));
    }

    public function testShouldSetSaveFlags()
    {
        $augeas = new Augeas("root", "", Augeas::AUGEAS_NONE);
        $this->assertEquals("overwrite", $augeas->get("/augeas/save"));

        $augeas = new Augeas("root", "", Augeas::AUGEAS_SAVE_NEWFILE);
        $this->assertEquals("newfile", $augeas->get("/augeas/save"));
        
        $augeas = new Augeas("root", "", Augeas::AUGEAS_SAVE_BACKUP);
        $this->assertEquals("backup", $augeas->get("/augeas/save"));
    }

}

?>
