<?php

/**
 *  Entity - Command
 *  @see /engine/core/api_entity.inc
 */

$entity = array(
    
    //      Fields
    'fields' => array(
        
        'id_thread' => array(
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
        
        'description' => array(
            'type' => 'string',
        ),
        
        'command' => array(
            'type' => 'string',
        ),
        
        'data' => array(
            'type' => 'binary',
        ),
        
        'datestamp' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
    ),
    
    //      Keys
    'keys' => array(
        'id_thread',
    ),
    
    //      GUID
    'guid' => array( 'title' ),
);


