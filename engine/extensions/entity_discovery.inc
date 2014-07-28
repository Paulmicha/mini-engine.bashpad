<?php

/**
 *  @file
 *  Entity - search configurable dirs for entities definition files,
 *  and include them.
 */

//      Load any candidate entities' custom processes in global scope
//      @evol 01:32 20/04/2014 - appcache in writeable dir ?
if ( !empty( $extensions[ 'entities' ][ 'settings' ][ 'lookup_paths' ]))
{
    //$entities_extension_files = list_files( 'config/entities', array( 'mask' => '`.\.module`i' ));
    $entities_extension_files = list_files( $extensions[ 'entities' ][ 'lookup_paths' ], array( 'mask' => '`.\.entity\.inc`i' ));
    if ( !empty( $entities_extension_files ))
        foreach( $entities_extension_files as $file )
            if ( file_exists( $file->realpath ))
                include $file->realpath;
}

