<?php

/**
 *  Entity - Server
 *  @see /engine/core/api_entity.inc
 */


$entity = array(
    
    //      Fields
    'fields' => array(
        
        'guid' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'title' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'hostname' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'description' => array(
            'type' => 'string',
        ),
        
        'data' => array(
            'type' => 'binary',
        ),
    ),
    
    //      GUID
    'guid' => array( 'title' ),
);


