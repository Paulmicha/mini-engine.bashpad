<?php

/**
 *  @file
 *  Test : reserved global vars
 *  @path /system/test/globals
 */

//      Init
$head_page_title = "List of reserved PHP variables in global scope";

$reserved_vars_html = <<<'EOD'
//      @see engine/settings.inc

//      Main HTML content string
$content = "";
$content_before = "";
$content_markup = "";
$content_after = "";

//      Raw query arguments (array of exploded /path/elements)
$request_arr = array();

//      System dynamic file includes path
$theme_path = "theme";
$context_handlers_path = "context";

//      Default includes
$content_include = "index";
$content_template_type = "page";
$content_template = $theme_path ."/index.tpl.php";
$template_file = $theme_path ."/page.tpl.php";

//      URL https
$http_s = "http://";

//      Extra navbar items HTML
$navbar_items = '';

//      Note : base_path() always MUST start and end with "/"
$base_path = base_path();

//      Theming-related variables
$body_id = '';
$head_extra_markup = '';
$body_extra_css_classes = '';
$head_page_title = "Mini-engine";
$short_page_title = "Mini-engine";

//      Log level
$log = array(
    'db' => 0,
);

//      Debug level
$debug = array(
    
    //'error' => 0,
    'error' => 1,
    
    'routing' => 0,
    //'routing' => 1,
    
    //'exec_time' => 1,
    'exec_time' => 1,
);
EOD;

$content .= view_source( $reserved_vars_html );


