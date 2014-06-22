<?php

/**
 *  @file
 *  Mini Engine Bootstrap
 */

//      Config
include 'engine/core/config/default_settings.inc';
include 'config/settings.inc';
include 'engine/core/config/php_runtime_settings.inc';

//      Engine
include 'engine/vendors.inc';
include 'engine/core/helpers.inc';
include 'engine/core/api.inc';

//      Optional core system routes
//      (mostly self-documenting pages and tests, DB log entries, etc.)
if ( $enable_system_routes ) {
    include 'engine/core/system_routing.inc';
}

//      Global context handler (always included)
include 'routing/global.inc';

//      debug
//print '<pre> $_SESSION = ';
//print print_r( $_SESSION, 1 );
//print '</pre>';
//print '<pre> $_REQUEST = ';
//print print_r( $_REQUEST, 1 );
//print '</pre>';
//die( 'test' );

//      Content
if ( !$unauthorized ) {
    include 'engine/core/http_request_engine.inc';
}

//      Debug : adding execution time in output
if ( $enable_debug && $debug[ 'exec_time' ])
{
    $content .= '<div class="container" style="padding:30px 0;">';
    $content .= '<small style="color:#AAA">';
    $content .= get_system_exec_time();
    $content .= '</small>';
    $content .= '</div>';
}

//      Render request result
include 'engine/core/theme_rendering.inc';


