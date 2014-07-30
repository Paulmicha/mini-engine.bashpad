<?php

/**
 *  @file
 *  Default extensions settings
 *  (extensions global settings var)
 */

//      System UI
$extensions[ 'system_ui' ] = array(
    'enabled' => false,
    //'allowed_paths' => '',
    'require' => array( 'theme_bootstrap' ),
);

//      Entities
$extensions[ 'entity' ] = array(
    'enabled' => false,
    'require' => array( 'db', 'system_routing' ),
    'settings' => array(
        'lookup_paths' => array( 'config/entities', 'engine/extensions/entities' ),
    ),
);

//      In this case, there's not going to be any file to include
//      but it's necessary to let it "passively" be enabled as a dependency
$extensions[ 'session' ] = array( 'enabled' => false );

//      Front-end dependencies, registered here as extensions
$extensions[ 'theme_jquery' ] = array(
    'enabled' => false,
    'settings' => array( 'v' => '1.11.1' ),
);
$extensions[ 'theme_bootstrap' ] = array(
    'enabled' => false,
    'require' => array( 'theme_jquery' ),
    //'settings' => array( 'v' => '3.2.0', 'bootswatch' => 'yeti' ),
    'settings' => array( 'v' => '2.3.2', 'bootswatch' => 'spacelab' ),
);


