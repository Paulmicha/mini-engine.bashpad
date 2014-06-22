<?php

/**
 *  @file
 *  Entity - Environment
 *  
 *  @see /engine/core/api_entity.inc
 */


$entity = array(
    
    //      Fields
    'fields' => array(
        
        'id_server' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
            'mandatory' => true,
        ),
        
        'guid' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'title' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'project' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'instance' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'url' => array(
            'type' => 'string',
        ),
        
        'description' => array(
            'type' => 'string',
        ),
        
        'data' => array(
            'type' => 'binary',
        ),
    ),
    
    //      Keys
    'keys' => array(
        'id_server',
    ),
    
    //      GUID
    //'guid' => array( 'title', 'project', 'instance' ),
    'guid' => array( 'project', 'server', 'instance' ),
);


