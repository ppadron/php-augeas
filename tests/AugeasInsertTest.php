<?php

require_once "PHPUnit/Framework/TestCase.php";

class AugeasInsertTest extends PHPUnit_Framework_TestCase
{

    public function setUp()
    {
	$this->augeas = new Augeas(dirname(__FILE__) . "/root", dirname(__FILE__) . "/lenses");
    }

    public function testShouldBeAbleToInsertNodesBefore()
    {
        $beforeFirstComment = "comment inserted before the first comment";
        $firstComment       = "first comment";
        
        // inserting before the first comment
        $this->augeas->insert("/files/etc/hosts/#comment", "#comment", Augeas::AUGEAS_INSERT_BEFORE);
        $this->augeas->set("/files/etc/hosts/#comment[1]", $beforeFirstComment);

        $this->assertEquals($beforeFirstComment, $this->augeas->get("/files/etc/hosts/#comment[1]"));
        $this->assertEquals($firstComment,       $this->augeas->get("/files/etc/hosts/#comment[2]"));

    }

    public function testShouldBeAbleToInsertNodesAfter()
    {
        $afterFirstComment  = "comment inserted after the first comment";
        $firstComment       = "first comment";

        // inserting after the NEW first comment
        $this->augeas->insert("/files/etc/hosts/#comment[1]", "#comment", Augeas::AUGEAS_INSERT_AFTER);
        $this->augeas->set("/files/etc/hosts/#comment[2]", $afterFirstComment);

        $this->assertEquals($afterFirstComment, $this->augeas->get("/files/etc/hosts/#comment[2]"));
    }

}

?>
