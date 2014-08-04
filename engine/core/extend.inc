<?php

/**
 *  @file
 *  Extensions : optionally include extra files
 *  @see settings files
 */


//      Will look for any ".inc" files corresponding to the global "$extensions"
if ( !empty( $extensions ))
{
    $inclusion_sorting = array();
    foreach( $extensions as $ext_name => $config )
    {
        //      Use of '#' as first character is reserved for "meta" data
        if ( substr( $ext_name, 0,1 ) == '#' ) {
            continue;
        }
        
        //      The 'weight' used to determine the inclusion order is optional.
        //      Defaults to 0.
        $weight = 0;
        if ( !empty( $config[ 'weight' ])) {
            $weight = $config[ 'weight' ];
        }
        
        if ( !empty( $config[ 'enabled' ]))
        {
            $files_to_include = extension_get_files( $ext_name );
            if ( !empty( $files_to_include ))
            {
                foreach( $files_to_include as $f ) {
                    $inclusion_sorting[ $weight ][] = $f->fullpath;
                }
            }
        }
    }
    
    //      Sort & include
    ksort( $inclusion_sorting );
    foreach( $inclusion_sorting as $includes )
    {
        foreach( $includes as $fullpath )
        {
            //      debug
            if ( !empty( $debug[ 'extensions' ]))
            {
                echo '<br/>including : <code>';
                echo $fullpath;
                echo '</code>';
            }
            
            include $fullpath;
        }
    }
}


