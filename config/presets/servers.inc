<?php

/**
 *  @file
 *  Default servers
 */

$files_arr = list_files( 'config/presets/servers', array( 'mask' => "`.+\.php`i" ));
if ( !empty( $files_arr ))
{
    foreach( $files_arr as $f )
    {
        //      Feedback
        $content .= "<br>Loading server defined in <code>". $f->path .'/'.$f->filename ."</code> ...";
        include $f->path .'/'.$f->filename;
        
        //      @todo 2013/09/27 13:16:16
    }
}
else
    $content .= alert( "No server files were found in <code>config/presets/servers</code> -> cannot generate default servers.", 'warning' );

//      Feedback
$content .= "<hr>";
$content .= alert( "Default servers processed." );


