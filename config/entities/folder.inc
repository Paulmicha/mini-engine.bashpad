<?php

/**
 *  Entity - Folder
 *  @see /engine/core/api_entity.inc
 */


$entity = array(
    
    //      Fields
    'fields' => array(
        
        'id_environment' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
        
        'guid' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'path' => array(
            'type' => 'string',
        ),
        
        'owner_group' => array(
            'type' => 'string',
            'length' => 64,
        ),
        
        'permissions' => array(
            'type' => 'string',
            'length' => 32,
        ),
    ),
    
    //      Keys
    'keys' => array(
        'id_environment',
    ),
);


