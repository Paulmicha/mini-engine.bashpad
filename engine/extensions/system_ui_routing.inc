<?php

/**
 *  @file
 *  System UI - routing
 */

//      Dispatch 'system' routes from extensions system routing directory
if ( arg( 0 ) == 'system' ) {
    $routing_handlers_path = 'engine/extensions/system_ui_routing';
}

