<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasInsertTest extends PHPUnit_Framework_TestCase
{

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
