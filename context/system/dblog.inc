<?php

/**
 *  @file
 *  Mini Engine
 *  DBLog - list entries
 *  
 *  @path /system/dblog
 *  @path /system/dblog/<type>
 */

//      Init
$head_page_title = "DBLog entries";
$namespace = check_plain( $request_arr[ 2 ]);


//-------------------------------------------------------------------------------------
//      Liste toutes les entrées du dblog

$content = latest_dblog_entries();



