<?php

/**
 *  @file
 *  Extensions : optionally include extra files
 *  @see settings files
 */


//      Will look for any ".inc" files corresponding to the global "$extensions"
if ( !empty( $extensions ))
{
    foreach( $extensions as $ext_name => $config )
    {
        if ( !empty( $config[ 'enabled' ]))
        {
            $files_to_include = extension_include( $ext_name );
            
            if ( !empty( $files_to_include ))
            {
                foreach( $files_to_include as $f )
                {
                    //      debug
                    if ( !empty( $debug[ 'extensions' ]))
                    {
                        echo '<br/>including : <code>';
                        echo $f->fullpath;
                        echo '</code>';
                    }
                    
                    include $f->fullpath;
                }
            }
        }
    }
}



/**
 *  "Autoload" extension files
 *  (Recursive)
 */
function extension_include( $ext_name, $is_dependency = false )
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
        'max_depth' => 1,
    ));
    
    //      Auto "resolve" dependencies
    if ( !empty( $extensions[ $ext_name ][ 'require' ]))
    {
        foreach( $extensions[ $ext_name ][ 'require' ] as $dep )
        {
            $extensions[ $dep ][ 'enabled' ] = true;
            $dependencies_arr = extension_include( $dep, true );
            
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


