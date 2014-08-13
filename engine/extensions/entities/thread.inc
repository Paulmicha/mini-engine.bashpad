<?php

/**
 *  Entity - Thread
 *  @see /engine/core/api_entity.inc
 */


$entity = array(
    
    //      Fields
    'fields' => array(
        
        'id_batch' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
        
        'state' => array(
            'type' => 'string',
            'length' => 32,
            'mandatory' => true,
        ),
        
        'preset' => array(
            'type' => 'integer',
            'size' => 'tiny',
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
        'id_batch',
        'preset',
        'state',
    ),
);


