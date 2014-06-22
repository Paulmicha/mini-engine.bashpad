<?php

/**
 *  @file
 *  debug
 */

//      Custom text files parser
$files_arr = list_files( 'config/import/custom', array( 'mask' => "`.+\.txt`i" ));
if ( !empty( $files_arr ))
{
    foreach( $files_arr as $f )
    {
        $structured_arr = servers_and_envs_text_parse( $f->path .'/'.$f->filename );
        
        //      debug
        krumo( $structured_arr );
    }
}


