<?php

/**
 *  @file
 *  Example Settings
 *  Rename this file "settings.inc"
 *  
 *  This is pure Global Scope Mayhem, as promised :P
 */


//      Base path
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
//      Extensions
//      Uncomment to enable (& configure)
//      @todo 2014/07/28 07:38:32 - in progress


$extensions[ 'system_ui' ][ 'enabled' ] = true;
$extensions[ 'entity' ][ 'enabled' ] = true;



//------------------------------------------
//      Content Defaults


//$page_title = "Hello world";
//$metadata[ 'site_name' ] = "My Project";
//$metadata[ 'head_page_title' ] = $page_title .' — '. $metadata[ 'site_name' ];
//$metadata[ 'description' ] = "Description of my project";
//$metadata[ 'lang' ] = 'en';
//$metadata[ 'locale' ] = 'en_EN';
//$metadata[ 'timezone' ] = 'UTC';



//------------------------------------------
//      Debug


//$enable_debug = true;
//$debug[ 'extensions' ] = 1;
//$debug[ 'routing' ] = 1;
//$debug[ 'tpl' ] = 1;
//$debug[ 'req' ] = 1;
//$debug[ 'exec_time' ] = 1;


