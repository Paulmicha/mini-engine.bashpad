<?php

/**
 *  @file
 *  Mini Engine
 *  DBLog - list entries
 *  
 *  @path /system/dblog
 *  @path /system/dblog/<type> (@todo)
 */

//      Init
$head_page_title = "DBLog entries";

//      @todo 00:13 20/04/2014
$namespace = check_plain( arg( 2 ));



//-------------------------------------------------------------------------------------
//      List dblog entries


$content = latest_dblog_entries();



