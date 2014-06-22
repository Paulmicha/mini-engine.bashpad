<?php

/**
 *  @file
 *  Mini Engine Settings
 *  
 *  This is pure Global Scope Mayhem, as promised :P
 */


//--------------------------------------------------------------------------------------
//      Features


$enable_db = false;
$enable_dblog = false;//      requires $enable_db
$enable_cache = false;//      requires $enable_db
$enable_tag = false;//      requires $enable_db
$enable_token = false;//      requires $enable_db
$enable_datatable = false;//      requires $enable_db
$enable_entity = false;//      requires $enable_db
$enable_ssh = false;//      requires $enable_db

$enable_session = false;
$enable_notification = false;//      requires $enable_session

$enable_date = false;
$enable_file = false;
$enable_log = false;

$enable_simple_htmldom = false;
$enable_form = false;//      requires $enable_simple_htmldom

$enable_form_theming = false;
$enable_form_multigroup = false;
$enable_form_file_selector = false;

$enable_system_routes = false;
$enable_debug = false;



//--------------------------------------------------------------------------------------
//      Database settings


$db_settings = array();
$db_active = 'default';

//      Each connection must be defined as follows (the 'default' key is mandatory)
$db_settings[ 'default' ] = array(
    
    'db_name' => "name",
    'db_user' => "login",
    'db_pass' => "pass",
    
    //      (optional, todo)
    //'db_driver' => "mysql",
    //'db_host' => "localhost",
);



//--------------------------------------------------------------------------------------
//      Base path


//      MUST start AND end with "/"
//      @see .htaccess @ line 13
//      Examples :
//          If you run this application in http://www.example.com/, then leave this "base path" at "/"
//          If in http://localhost/mini-engine, then use "/mini-engine/"
//          If in http://192.168.1.2/mini-engine/www, then use "/mini-engine/www/"
//          etc.
$base_path = "/";



//--------------------------------------------------------------------------------------
//      Mini engine settings (global scope "reserved" vars)


$content = "";
$content_before = "";
$content_markup = "";
$content_after = "";
$footer_extra_js = "";
$request_arr = array();
$routing_handlers_path = "routing";//       dir to scan for routing handlers
$http_s = "http://";
if ( @$_SERVER[ 'HTTPS' ] == "on" ) {
    $http_s = "https://";
}
$navbar_items = '';

//      Theming-related
$max_routing_depth = 9;
$theme_path = "theme";//       dir to scan for template files & assets
$content_include = "index";
$content_template_type = "page";
$content_template = $theme_path ."/index.tpl.php";
$template_file = $theme_path ."/page.tpl.php";

//      Default page content (home)
$body_id = '';
$head_extra_markup = '';
$body_extra_css_classes = '';

$lang = 'en';
$site_name = "Mini-engine";
$head_page_title = "Mini-engine";
$short_page_title = "Mini-engine";
$meta_description = "Mini-engine";

$unauthorized = false;

//      Log level
$log = array(
    'db' => 0,
);



//--------------------------------------------------------------------------------------
//      Debug


//      Types of debug
$debug = array(
    
    //      PDO queries exceptions
    //'error' => 0,
    'error' => 1,
    
    //      Included files (theming)
    'routing' => 0,
    //'routing' => 1,
    
    //      Execution time (at the bottom of every page)
    //'exec_time' => 0,
    'exec_time' => 1,
    
    //      SSH commands killswitch
    'prevent_ssh_exec' => 0,
    //'prevent_ssh_exec' => 1,
);


