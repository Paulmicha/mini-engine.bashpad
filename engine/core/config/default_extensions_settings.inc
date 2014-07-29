<?php

/**
 *  @file
 *  Default extensions settings
 *  (extensions global settings var)
 */

//      Extensions global settings var
$extensions = array(
    '#settings' => array(
        'lookup_paths' => array( 'engine/extensions', 'config/extensions' ),
    ),
);

$extensions[ 'entity' ] = array(
    'enabled' => false,
    'require' => array( 'db' ),
    'settings' => array(
        'lookup_paths' => array( 'config/entities', 'engine/extensions/entities' ),
    ),
);

$extensions[ 'theme_bootstrap' ] = array( 'enabled' => false );

//      In this case, there's not going to be any file to include
//      but it's necessary to let it "passively" be enabled as a dependency
$extensions[ 'session' ] = array( 'enabled' => false );


