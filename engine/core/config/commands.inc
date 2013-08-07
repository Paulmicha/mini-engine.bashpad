<?php

/**
 *  @file
 *  Default commands
 */

//      Set default commands
$default_commands = array(
    
    //      For testing : listing all contents of a folder
    //      "ls -la"
    array(
        'title' => "Listing folder contents (homedir)",
        'description' => "This will display a list of files with permissions, group & owner details, usually in homedir (or the default folder we are in when logging in with SSH)",
        'command' => "ls -la",
        'datestamp' => datestamp(),
    ),
    
    //      For testing : listing all contents of a folder in project root dir
    array(
        'title' => "Listing folder contents (Project Root)",
        'description' => "This will display a list of files with permissions, group & owner details, inside the Project Root folder (dynamic token with tag)",
        'command' => "cd [folder:project root] && ls -la",
        'datestamp' => datestamp(),
    ),
);

//      @evol 2013/07/29 17:34:52 - error checking
foreach( $default_commands as $command )
    $id_command = insert_if_no_existing_record( 'command', $command, array( 'unique_field_name' => 'title' ));


//      Feedback
$content .= alert( "Default commands have been created.", 'success' );


