<?php

/**
 *  @file
 *  Cache clear
 *  @path /system/cache-clear/<namespace>
 */

//      Init
$head_page_title = "Clear cache";
$namespace = check_plain( arg( 2 ));

//      Failsafe
if ( empty( $namespace ))
    $content .= alert( "Missing 'namespace' as argument" );
else
{
    //      Clear cache
    if ( cache_clear( $namespace ))
        $content .= alert( "Cache '$namespace' successfully cleared.", 'success' );
    else
        $content .= alert( "<strong> Error </strong> when clearing cache '$namespace'", 'error' );
}

