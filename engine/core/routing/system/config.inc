<?php

/**
 *  @file
 *  Configuration management
 *  
 *  @path /system/config/load
 *  @path /system/config/save
 */

$operation = arg( 2 );
$head_page_title = "Configuration management";


//      Failsafe
if ( empty( $operation )) {
    $content .= alert( "Missing operation to execute" );
}
else {
    include "$routing_handlers_path/system/config/$operation.inc";
}


