<?php

/**
 *  @file
 *  DB (un)installation / presets import
 *  
 *  @path /system/setup/install
 *  @path /system/setup/uninstall
 *  @path /system/setup/load-config
 */

$operation = arg( 2 );
$head_page_title = "Setup operations";


//      Failsafe
if ( empty( $operation )) {
    $content .= alert( "Missing operation to execute" );
}
else {
    include "$routing_handlers_path/system/setup/$operation.inc";
}


