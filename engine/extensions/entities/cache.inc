<?php

/**
 *  @file
 *  Entity definition - Cache
 *  
 *  @see /engine/core/api_entity.inc
 */

$entity = array(
    
    //      Fields
    'fields' => array(
        
        'title' => array(
            'type' => 'string',
            'length' => 255,
        ),
        
        'namespace' => array(
            'type' => 'string',
            'length' => 128,
        ),
        
        'datestamp' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
        
        'datestamp_expire' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
        
        'data' => array(
            'type' => 'binary',
            'size' => 'big',
        ),
    ),
);


