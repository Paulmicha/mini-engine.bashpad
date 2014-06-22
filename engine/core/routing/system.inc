<?php

/**
 *  @file
 *  System (mini-engine core functions - "routing" dispatcher file)
 *  
 *  @path /system/<operation>
 *  @path /system/<operation>/...
 */

//      Init
$head_page_title = "System";
$operation = check_plain( arg( 1 ));

//      Custom dispatch
if ( empty( $operation )) {
    $content .= alert( "No operation selected." );
}
else if ( file_exists( "$routing_handlers_path/system/$operation.inc" )) {
    include "system/$operation.inc";
}
else {
    send_404();
}


