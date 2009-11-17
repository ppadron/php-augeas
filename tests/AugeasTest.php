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

    public function testShouldParseHostsFile()
    {
        $augeas = new Augeas("root");
        $this->assertEquals("127.0.0.1", $augeas->get("/files/etc/hosts/1/ipaddr"));
        $this->assertEquals("localhost", $augeas->get("/files/etc/hosts/1/canonical"));
        $this->assertEquals("localhost.localdomain", $augeas->get("/files/etc/hosts/1/alias"));
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

    public function testShouldBeAbleToInsertNodesBefore()
    {
        $beforeFirstComment = "comment inserted before the first comment";
        $firstComment       = "first comment";
        
        $augeas = new Augeas("root");

        // inserting before the first comment
        $augeas->insert("/files/etc/hosts/comment[1]", "comment", Augeas::AUGEAS_INSERT_BEFORE);
        $augeas->set("/files/etc/hosts/comment[1]", $beforeFirstComment);

        $this->assertEquals($beforeFirstComment, $augeas->get("/files/etc/hosts/comment[1]"));
        $this->assertEquals($firstComment,       $augeas->get("/files/etc/hosts/comment[2]"));

    }

    public function testShouldBeAbleToInsertNodesAfter()
    {
        $afterFirstComment  = "comment inserted after the first comment";
        $firstComment       = "first comment";

        $augeas = new Augeas("root");

        // inserting after the NEW first comment
        $augeas->insert("/files/etc/hosts/comment[1]", "comment", Augeas::AUGEAS_INSERT_AFTER);
        $augeas->set("/files/etc/hosts/comment[2]", $afterFirstComment);

        $this->assertEquals($afterFirstComment,  $augeas->get("/files/etc/hosts/comment[2]"));
    }



}

?>
