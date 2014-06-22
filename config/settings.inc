<?php

/**
 *  @file
 *  Custom Settings
 *  
 *  This is pure Global Scope Mayhem, as promised :P
 */


//      Everything depends on this
//      MUST start AND end with "/"
//      @see .htaccess @ line 13
//      Examples :
//          If you run this application in http://www.example.com/, then leave this "base path" at "/"
//          If in http://localhost/mini-engine, then use "/mini-engine/"
//          If in http://192.168.xxx.xxx/mini-engine/www, then use "/mini-engine/www/"
//          etc.
$base_path = '/';
//$base_path = '/mini-engine/';
//$base_path = '/mini-engine/www/';



//------------------------------------------
//      Features


$enable_debug = true;
$enable_db = true;
$enable_dblog = true;
$enable_cache = true;
$enable_tag = true;
$enable_token = true;
$enable_datatable = true;
$enable_entity = true;
$enable_ssh = true;
//$enable_session = true;
$enable_notification = true;
//$enable_date = true;
//$enable_file = true;
//$enable_log = true;
$enable_form = true;
$enable_form_theming = true;
$enable_form_file_selector = true;
$enable_form_multigroup = true;
//$enable_simple_htmldom = true;
$enable_system_routes = true;



//------------------------------------------
//      DB settings


$db_settings[ 'default' ] = array(
    'db_name' => "***",
    'db_user' => "***",
    'db_pass' => "***",
);



//------------------------------------------
//      Debug


$debug[ 'error' ] = 1;
//$debug[ 'installation' ] = 1;
//$debug[ 'routing' ] = 1;
$debug[ 'exec_time' ] = 1;
//$debug[ 'fake_ssh_exec' ] = 1;



//------------------------------------------
//      Content Defaults


$lang = 'en';
$site_name = "Site name";
$head_page_title = "Head title";
$short_page_title = "Page title";
$meta_description = "Short description";



//------------------------------------------
//      Misc


//      Hardcore setting
//set_time_limit( 0 );
    

