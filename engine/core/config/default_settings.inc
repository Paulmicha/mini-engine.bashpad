<?php

/**
 *  @file
 *  Mini Engine default settings
 *  (global scope "reserved" vars)
 *  
 *  This is pure Global Scope Mayhem, as promised :P
 */

$mini_engine_version = "1.0.0";


//      Base path
//      MUST start AND end with "/"
//      @see .htaccess @ line 13
//      Examples :
//          If you run this application in http://www.example.com/, then leave this "base path" at "/"
//          If in http://localhost/mini-engine, then use "/mini-engine/"
//          If in http://192.168.1.2/mini-engine/www, then use "/mini-engine/www/"
//          etc.
$base_path = "/";


//      Extensions global
//      (moved to their own separate file)
$extensions = array(
    '#settings' => array(
        'lookup_paths' => array( 'engine/extensions', 'config/extensions' ),
    ),
);
include 'engine/extensions/config/default_settings.inc';


//      Routing globals
$request_arr = array();
$routing_max_depth = 9;//           max number of parts (default deepest URL ex: http://example.com/up/to/nine/elements/separated/by/slashes/by/default)
$routing_path = "routing";//        default dir to scan for routing handlers
$routing_filename = "index";


//      Template globals
$theme_path = "theme";//            dir to scan for template files & assets
$base_tpl = "page";
$container_tpl = "$theme_path/$base_tpl.tpl.php";
$content_tpl = "$theme_path/index.tpl.php";
$content_tpl_extensions_arr = array( '.tpl.php', '.html' );//       file extensions allowed for use as content tpl


//      Content globals
$page_title = "Welcome home";
$content = "";
$content_rendered = "";


//      Spaces :
//      Contains HTML markup for use in tpl files.
//      (bridges templates & content ~ expresses what goes where)
//      @example theme/page.tpl.php
$spaces = array();
$spaces[ 'head' ] = '';
$spaces[ 'page_top' ] = '';
$spaces[ 'page_bottom' ] = '';


//      Metadata global
$metadata = array();
$metadata[ 'site_name' ] = "Mini-engine";
$metadata[ 'head_page_title' ] = $page_title .' — '. $metadata[ 'site_name' ];
$metadata[ 'description' ] = "Mini-engine";
$metadata[ 'lang' ] = 'en';
$metadata[ 'locale' ] = 'en_EN';
$metadata[ 'timezone' ] = 'UTC';//        ex: 'Europe/Paris', etc.


//      Misc. globals
$html_attr = array(
    'lang' => $metadata[ 'lang' ],
    'class' => 'no-js',
);
$body_attr = array();

$http_s = "http://";
if ( @$_SERVER[ 'HTTPS' ] == "on" ) {
    $http_s = "https://";
}


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


