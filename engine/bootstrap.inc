<?php

/**
 *  @file
 *  Mini Engine Bootstrap
 */

//      Setup
include 'config/settings.inc';

//      Engine
include 'vendors.inc';
include 'core/api.inc';
include 'core/helpers.inc';

//      Global context (always included)
include 'context/global.inc';

//      debug
//print '<pre> $_SESSION = ';
//print print_r( $_SESSION, 1 );
//print '</pre>';
//print '<pre> $_REQUEST = ';
//print print_r( $_REQUEST, 1 );
//print '</pre>';
//die( 'test' );

//      Content
if ( !$unauthorized )
    include 'core/http_request_engine.inc';

//      Render request result
include 'core/theme_rendering.inc';

//      Performance measure
if ( $debug[ 'exec_time' ])
{
    print '<div class="container" style="padding:30px 0;">';
    print '<small style="color:#AAA">';
    print get_system_exec_time();
    print '</small>';
    print '</div>';
}


