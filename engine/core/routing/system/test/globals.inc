<?php

/**
 *  @file
 *  Test : reserved global vars
 *  @path /system/test/globals
 */

//      Init
$head_page_title = "List of reserved PHP variables in global scope";

$reserved_vars_html = <<<'EOD'
//      @see config/settings.inc

$content = "";
$content_before = "";
$content_markup = "";
$content_after = "";
$footer_extra_js = "";
$request_arr = array();
$routing_handlers_path = "routing";
$http_s = "http://";
$navbar_items = '';

//      Theming-related
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
$unauthorized = false;

//      Log level
$log = array(
    'db' => 0,
);

//      Types of debug
$debug = array(
    
    //      PDO queries exceptions
    'error' => 0,
    //'error' => 1,
    
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

EOD;

$content .= view_source( $reserved_vars_html );


