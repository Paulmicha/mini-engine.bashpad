<?php

/**
 *  @file
 *  Test : dblog
 *  @path /system/test/dblog
 */

//      Init
$head_page_title = "Test dblog";

//      Write a db log entry
dblog( "testing", "A message ". time() );

//      Write another one with vars
dblog( "testing", "A message ". time(), array( 'data' => array( 'my_var' => "My Value" )));

$code = <<<'EOT'
//      Write a db log entry
dblog( "testing", "A message ". time() );

//      Write another one with data
dblog( "testing", "A message ". time(), array( 'data' => array( 'my_var' => "My Value" )));
EOT;

$content .= view_source( $code );

$content .= "<hr />";
$content .= "<h2>Result : </h2>";
$content .= latest_dblog_entries();
