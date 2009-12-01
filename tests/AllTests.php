<?php

require_once "PHPUnit/Framework.php";
require_once dirname(__FILE__) . "/AugeasTest.php";
require_once dirname(__FILE__) . "/AugeasGetTest.php";
require_once dirname(__FILE__) . "/AugeasMatchTest.php";
require_once dirname(__FILE__) . "/AugeasInsertTest.php";
require_once dirname(__FILE__) . "/AugeasMvTest.php";

class AllTests
{

    public static function suite()
    {
        $suite = new PHPUnit_Framework_TestSuite("PHPUnit");
        $suite->addTestSuite("AugeasTest");
        $suite->addTestSuite("AugeasInsertTest");
        $suite->addTestSuite("AugeasMvTest");
        $suite->addTestSuite("AugeasGetTest");
        $suite->addTestSuite("AugeasMatchTest");
        return $suite;
    }

}

?>
