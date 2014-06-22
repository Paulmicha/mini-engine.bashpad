<?php

/**
 *  @file
 *  Entity API
 */


//      Load any candidate entities' custom processes in global scope
//      @evol 01:32 20/04/2014 - appcache in writeable dir
$entities_extension_files = list_files( 'config/entities', array( 'mask' => '`.\.module`i' ));
if ( !empty( $entities_extension_files ))
    foreach( $entities_extension_files as $file )
        if ( file_exists( $file->realpath ))
            include $file->realpath;



/**
 *  Simple GUID
 */
function entity_guid( $type, $entity, $options = array())
{
    $guid = '';
    
    //      Overridable defaults
    $options += array(
        'separator' => '-',
        'conversion_callback' => 'str_clean'
    );
    extract( $options );
    
    $metadata = entity_metadata( $type, $options );
    
    //      debug
    //krumo( $type );
    //krumo( $entity );
    //krumo( $metadata );
    
    if ( !empty( $metadata[ 'guid' ]) && !empty( $entity ) && function_exists( $conversion_callback ))
    {
        //      GUID is formed by a concatenation of fields
        if ( is_array( $metadata[ 'guid' ]))
        {
            foreach( $metadata[ 'guid' ] as $i => $field )
            {
                if ( !empty( $entity[ $field ]))
                {
                    if ( $i > 0 ) {
                        $guid .= $separator . $conversion_callback( $entity[ $field ]);
                    }
                    else {
                        $guid .= $conversion_callback( $entity[ $field ]);
                    }
                }
            }
        }
        
        //      GUID is formed by a custom callback
        else if ( is_string( $metadata[ 'guid' ]) && function_exists( $metadata[ 'guid' ])) {
            $guid = $metadata[ 'guid' ]( $entity );
        }
    }
    
    //      debug
    //krumo( $guid );
    
    return $guid;
}



/**
 *  Simple Entity "metadata" provider
 *  (per entity type or for all entity types found)
 *  
 *  Expected behavior :
 *      • include any *.inc files in /config/entities, or just "$type.inc" if optional $type arg. is not empty
 *      • these files must define (in a PHP variable) an associative array named $entity
 *      • $entity[ 'fields' ] must provide fields structure ("ORM" like, for storage structure ~ DB ops)
 *      • (optional) $entity[ 'guid' ] should contain either fields array, or a callback function to provide a unique (varchar 255 string) identifier
 *      • (optional) $entity[ 'relations' ] ex: has_tag table
 *      • (optional) $entity[ 'codecs' ] could provide codec callbacks, for simple import / export feature
 *  
 *  @param (optional) String $type : system name of entity type
 *  @param (optional) Array $options
 *  @return Array : contains guid (fields array or callback) & fields structure ~ keys (formatted for DB Insertion)
 */
function entity_metadata( $type = null, $options = array())
{
    $mask = '`.\.inc`i';
    static $metadata_arr = array();
    
    if ( !empty( $type ) && empty( $options ) && !empty( $metadata_arr[ $type ])) {
        return $metadata_arr[ $type ];
    }
    
    //      Overridable defaults
    $options += array(
        'paths' => array( 'config/entities', 'engine/core/entities' ),
    );
    
    extract( $options );
    
    if ( !empty( $type )) {
        $mask = '`'. $type .'\.inc`i';
    }
    
    $files_arr = list_files( $paths, array( 'mask' => $mask ));
    
    //      debug
    //krumo( $type );
    //krumo( $files_arr );
    
    if ( !empty( $files_arr ))
        foreach( $files_arr as $file )
            if ( file_exists( $file->fullpath ))
            {
                include $file->fullpath;
                $entity_name = str_replace( '.inc', '', $file->filename );
                
                //      debug
                //krumo( $file->fullpath );
                //krumo( $entity );
                
                if ( !empty( $entity ))
                    foreach( $entity as $definition => $data )
                        $metadata_arr[ $entity_name ][ $definition ] = $data;
            }
    
    if ( !empty( $type )) {
        return $metadata_arr[ $type ];
    }
    
    return $metadata_arr;
}


