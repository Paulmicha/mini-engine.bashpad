<?php

$extensions[ 'entity' ] = array(
    'require' => array( 'db', 'system_ui' ),
    'settings' => array(
        'lookup_paths' => array( 'config/entities', 'engine/extensions/entities' ),
    ),
);
