<?php

/**
 *  @file
 *  Mini Engine Settings
 */


//--------------------------------------------------------------------------------------
//      PHP settings


error_reporting( E_ALL ^ E_NOTICE );
//error_reporting( E_ERROR | E_WARNING | E_PARSE | E_COMPILE_ERROR );
//error_reporting( 0 );

//		Fix IE sessions inside iframes
//      @see http://stackoverflow.com/questions/5257983/what-is-headerp3p-cp-cao-psa-our
header( 'P3P: CP="CAO PSA OUR"' );
header( 'Content-Type: text/html; charset=utf-8' );
session_start();
mb_internal_encoding( "UTF-8" );

//setlocale( LC_ALL, 'fr_FR' );
setlocale( LC_ALL, 'en_EN' );

//      Hardcore setting
set_time_limit( 0 );



//--------------------------------------------------------------------------------------
//      Database settings


/**
 *  DB settings 'getter' function
 */
function db_get_credentials()
{
    //      Replace those with your own DB settings
    return array(
        'db_name' => "mini-engine_www",
        'db_user' => "mini_engine",
        'db_pass' => "uym9NFbpt9wQ6WEt"
    );
}



//--------------------------------------------------------------------------------------
//      Mini engine (global scope "reserved" vars) settings


$content = "";
$content_before = "";
$content_markup = "";
$content_after = "";
$request_arr = array();
$context_handlers_path = "context";
$http_s = "http://";
if ( $_SERVER[ 'HTTPS' ] == "on" ) {
    $http_s = "https://";
}
$navbar_items = '';

//      Theming-related
//      Replace this string with your base path
//      Note : base_path() always MUST start and end with "/"
function base_path() { return "/mini-engine/www/"; }
$base_path = base_path();
$theme_path = "theme";
$content_include = "index";
$content_template_type = "page";
$content_template = $theme_path ."/index.tpl.php";
$template_file = $theme_path ."/page.tpl.php";

//      Default page content (home)
$body_id = '';
$head_extra_markup = '';
$body_extra_css_classes = '';
$head_page_title = "Mini-engine";
$short_page_title = "Mini-engine";

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
    
    //      Execution time (at the bottom every page)
    //'exec_time' => 0,
    'exec_time' => 1,
    
    //      Don't execute SSH commands
    'prevent_ssh_exec' => 0,
    //'prevent_ssh_exec' => 1,
);


