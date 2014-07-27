<?php

/**
 *  @file
 *  Custom Settings (edit this file as needed)
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
//$base_path = '/';
$base_path = '/mini-engine/v1.0.0-alpha/';



//------------------------------------------
//      Debug


$enable_debug = true;

//$debug[ 'routing' ] = 1;
//$debug[ 'tpl' ] = 1;
//$debug[ 'req' ] = 1;
//$debug[ 'exec_time' ] = 1;



//------------------------------------------
//      Content Defaults


$lang = 'en';
$locale = 'en_EN';
$timezone = 'UTC';

$site_name = "Mini Engine";
$head_page_title = "Mini Engine";
$page_title = "Mini Engine";
$meta_description = "Description of Mini-Engine";


