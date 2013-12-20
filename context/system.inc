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

//      Failsafe
if ( empty( $operation ))
    $content .= alert( "No operation selected." );
else
{
    //      Including corresponding test file
    include "system/$operation.inc";
}


