<?php

/**
 *  @file
 *  Default commands
 */

//      Set default commands
//      Fetch all php files in 'engine/core/config/bash'
//      They must contain a single variable $command containing an array of bash commands data
$files_arr = list_files( 'engine/core/config/bash', array( 'mask' => "`.+\.php`i" ));
if ( !empty( $files_arr ))
{
    foreach( $files_arr as $f )
    {
        //      Feedback
        $content .= "<br>Loading command defined in <code>". $f->path .'/'.$f->filename ."</code> ...";
        include $f->path .'/'.$f->filename;
        
        //      Error checking
        if ( !empty( $command ))
        {
            $id_command = insert_if_no_existing_record( 'command', $command, array( 'unique_field_name' => 'title' ));
            
            //      Feedback
            if ( !empty( $id_command ))
                $content .= alert( "Command '". $command[ 'title' ] ."' successfully processed.", 'success' );
            else
                $content .= alert( "Error : could not insert command. That's all I know.", 'error' );
        }
        else
            $content .= alert( "This file does not seem to define a valid command.", 'warning' );
    }
}
else
    $content .= alert( "No commands files were found in <code>engine/core/config/bash</code> -> cannot generate default commands.", 'warning' );

//      Feedback
$content .= "<hr>";
$content .= alert( "Default commands processed.", 'success' );


