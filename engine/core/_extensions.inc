<?php

/**
 *  @file
 *  Extensions : optionally include extra files
 *  @see settings files
 */


/**
 *  Initial extensions list & default configuration
 */
function extensions_list()
{
    global $extensions;
    
    //      Look for a "config" folder inside each lookup path
    foreach( $extensions[ '#settings' ][ 'lookup_paths' ] as $path )
    {
        $files_to_include = list_files( "$path/config", array(
            'mask' => '`\.default_settings\.inc$`i',
            'max_depth' => 1,
        ));
        
        if ( !empty( $files_to_include ))
            foreach( $files_to_include as $f )
                include $f->fullpath;
    }
}



/**
 *  Discover extension files + add & enable dependencies
 *  (Recursive)
 */
function extension_get_files( $ext_name, $is_dependency = false )
{
    global $debug;
    global $extensions;
    
    //      Optimze for potential cross-dependencies
    static $once_arr;
    if ( !is_array( $once_arr )) {
        $once_arr = array();
    }
    else if ( !empty( $once_arr[ $ext_name ])) {
        return;
    }
    $once_arr[ $ext_name ] = true;
    
    //      debug
    if ( !empty( $debug[ 'extensions' ]))
    {
        echo $is_dependency ? '<br/> └ dependency : <code>' : '<br/>requested : <code>';
        echo $ext_name;
        echo '</code>';
    }
    
    //      Naming convention : will include any "namespace.inc" & "namespace_subname.inc" files
    $files_to_include = list_files( $extensions[ '#settings' ][ 'lookup_paths' ], array(
        'mask' => '`^'. $ext_name .'\.inc$|^'. $ext_name .'_.+\.inc$`i',
        'max_depth' => 0,
    ));
    
    //      Auto "resolve" dependencies
    if ( !empty( $extensions[ $ext_name ][ 'require' ]))
    {
        foreach( $extensions[ $ext_name ][ 'require' ] as $dep )
        {
            //      Mark as enabled
            $extensions[ $dep ][ 'enabled' ] = true;
            $dependencies_arr = extension_get_files( $dep, true );
            
            if ( !empty( $dependencies_arr ))
            {
                foreach( $dependencies_arr as $f ) {
                    $is_dependency ? array_push( $files_to_include, $f ) : array_unshift( $files_to_include, $f );
                }
            }
        }
    }
    
    return $files_to_include;
}


