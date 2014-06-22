<?php

/**
 *  Entity - Batch Step
 *  @see /engine/core/api_entity.inc
 */

$entity = array(
    
    //      Fields
    'fields' => array(
        
        'id_batch' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
            'mandatory' => true,
        ),
        
        'id_command' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
            'mandatory' => true,
        ),
        
        'id_environment' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
            'mandatory' => true,
        ),
        
        'title' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'step' => array(
            'type' => 'integer',
            'size' => 'tiny',
            'unsigned' => true,
        ),
    ),
    
    //      Keys
    'keys' => array(
        'id_batch',
        'id_command',
        'id_environment',
    ),
);


