<?php

/**
 *  @file
 *  Mini Engine default settings
 *  
 *  This is pure Global Scope Mayhem, as promised :P
 */


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

//      Routing & Theming-related
$routing_max_depth = 9;
$theme_path = "theme";//       dir to scan for template files & assets
$routing_handler_filename = "index";
$content_tpl_type = "page";
$container_tpl = $theme_path ."/page.tpl.php";
$content_tpl = $theme_path ."/index.tpl.php";
$content_tpl_extensions_arr = array( '.tpl.php', '.html' );//       extensions to use as content tpl

//      Default page content (home)
$body_id = '';
$head_extra_markup = '';
$body_extra_css_classes = '';

$lang = 'en';
$locale = 'en_EN';
$timezone = 'UTC';//        ex: 'Europe/Paris', etc.

$site_name = "Mini-engine";
$head_page_title = "Mini-engine";
$page_title = "Mini-engine";
$meta_description = "Mini-engine";



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


