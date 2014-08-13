<?php

/**
 *  Entity - Batch
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
        
        'title' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'description' => array(
            'type' => 'string',
        ),
        
        'datestamp' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
        
        'data' => array(
            'type' => 'binary',
        ),
    ),
    
    //      Keys
    'keys' => array(
        'id_thread',
    ),
);


