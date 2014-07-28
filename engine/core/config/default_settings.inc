<?php

/**
 *  @file
 *  Mini Engine default settings
 *  (global scope "reserved" vars)
 *  
 *  This is pure Global Scope Mayhem, as promised :P
 */


//      Base path
//      MUST start AND end with "/"
//      @see .htaccess @ line 13
//      Examples :
//          If you run this application in http://www.example.com/, then leave this "base path" at "/"
//          If in http://localhost/mini-engine, then use "/mini-engine/"
//          If in http://192.168.1.2/mini-engine/www, then use "/mini-engine/www/"
//          etc.
$base_path = "/";


//      Extensions
$extensions = array();

//      @todo 2014/07/28 07:54:51 - to complete
$extensions[ 'entity' ] = array(
    'enabled' => false,
    'require' => array( 'db' ),
    'settings' => array(
        'lookup_paths' => array( 'config/entities', 'engine/extensions/entities' ),
    ),
);

//      Routing globals
$request_arr = array();
$routing_max_depth = 9;
$routing_handlers_path = "routing";//       dir to scan for routing handlers
$routing_handler_filename = "index";


//      Template globals
$theme_path = "theme";//       dir to scan for template files & assets
$content_tpl_type = "page";
$container_tpl = $theme_path ."/page.tpl.php";
$content_tpl = $theme_path ."/index.tpl.php";
$content_tpl_extensions_arr = array( '.tpl.php', '.html' );//       extensions to use as content tpl

$body_id = '';
$head_extra_markup = '';
$body_extra_css_classes = '';


//      Content globals
$content = "";
$content_before = "";
$content_markup = "";
$content_after = "";

$navbar_items = '';
$footer_extra_js = "";

$site_name = "Mini-engine";
$head_page_title = "Mini-engine";
$page_title = "Mini-engine";
$meta_description = "Mini-engine";


//      Misc. globals
$lang = 'en';
$locale = 'en_EN';
$timezone = 'UTC';//        ex: 'Europe/Paris', etc.

$http_s = "http://";
if ( @$_SERVER[ 'HTTPS' ] == "on" ) {
    $http_s = "https://";
}



//--------------------------------------------------------------------------------------
//      Debug


$enable_debug = false;//  NB: enables krumo & php error reporting

//      Types of debug
$debug = array(
    
    //      Included files (theming)
    'routing' => 0,
    //'routing' => 1,
    'tpl' => 0,
    //'tpl' => 1,
    'req' => 0,
    //'req' => 1,
    
    //      Execution time (at the bottom of every page)
    'exec_time' => 0,
    //'exec_time' => 1,
);


