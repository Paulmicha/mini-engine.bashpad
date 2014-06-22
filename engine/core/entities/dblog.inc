<?php

/**
 *  @file
 *  Entity definition - DB log
 *  
 *  @see /engine/core/api_entity.inc
 */

$entity = array(
    
    //      Fields
    'fields' => array(
        
        'type' => array(
            'type' => 'string',
            'length' => 64,
        ),
        
        'message' => array(
            'type' => 'string',
            'size' => 'long',
        ),
        
        'severity' => array(
            'type' => 'integer',
            'length' => 3,
            'unsigned' => true,
        ),
        
        'data' => array(
            'type' => 'binary',
        ),
        
        'location' => array(
            'type' => 'string',
        ),
        
        'referer' => array(
            'type' => 'string',
        ),
        
        'hostname' => array(
            'type' => 'string',
            'length' => 128,
        ),
        
        'timestamp' => array(
            'type' => 'integer',
            'length' => 11,
            'unsigned' => true,
        ),
        
        'datestamp' => array(
            'type' => 'integer',
            'size' => 'big',
            'unsigned' => true,
        ),
    ),
    
    
    //      Keys
    'keys' => array(
        'type',
        'severity',
    ),
    
    
    //      Relations
    'relations' => array(
        'has_dblog' => array(
            //      (no additional fields)
        ),
    ),
);


