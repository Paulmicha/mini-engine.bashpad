<?php

/**
 *  @file
 *  Entity definition - Tag
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
        
        'description' => array(
            'type' => 'string',
        ),
        
        'data' => array(
            'type' => 'binary',
        ),
    ),
    
    
    //      Relations
    'relations' => array(
        'has_tag' => array(
            
            //      Additional fields
            'fields' => array(
                'weight' => array(
                    'type' => 'integer',
                    'size' => 'tiny',
                    'unsigned' => true,
                ),
            ),
        ),
    ),
);


