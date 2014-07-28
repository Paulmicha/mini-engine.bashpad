<?php

/**
 *  @file
 *  Extensions : optional extra functions
 *  @see settings files
 */


//      Will look for any ".inc" files corresponding to the global "$extensions"
if ( !empty( $extensions ))
{
    foreach( $extensions as $ext_name => $config )
    {
        if ( !empty( $config[ 'enabled' ])) {
            extension_include( $ext_name );
        }
    }
}



/**
 *  "Autoload" extension files
 *  (Recursive)
 */
function extension_include( $ext_name )
{
    //      Optimze for potential cross-dependencies
    static $once_arr;
    if ( !is_array( $once_arr )) {
        $once_arr = array();
    }
    else if ( !empty( $once_arr[ $ext_name ])) {
        return;
    }
    $once_arr[ $ext_name ] = true;
    
    //      @todo 2014/07/28 07:05:31 - fix regex :
    /*
    including : engine/extensions/entity.inc
    including : engine/extensions/entity_discovery.inc
    including : engine/extensions/entity_storage.inc
    including : engine/extensions/db.inc
    including : engine/extensions/dblog.inc                 <--- NOT good
    including : engine/extensions/dblog_debug.inc           <--- NOT good
    including : engine/extensions/dblog_ui.inc              <--- NOT good
    */
    $files_to_include = list_files( 'engine/extensions', array( 'mask' => '`^'. $ext_name .'_?`i' ));
    
    foreach( $files_to_include as $f )
    {
        //      debug
        //echo '<br/>including : <code>';
        //echo $f->fullpath;
        //echo '</code>';
        
        include $f->fullpath;
    }
    
    //      Auto "resolve" dependencies
    if ( !empty( $GLOBALS[ 'extensions' ][ $ext_name ][ 'require' ]))
    {
        foreach( $GLOBALS[ 'extensions' ][ $ext_name ][ 'require' ] as $dep ) {
            extension_include( $dep );
        }
    }
}


