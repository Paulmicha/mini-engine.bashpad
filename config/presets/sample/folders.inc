<?php

/**
 *  @file
 *  Sample folders
 *  
 *  @requires Sample environment import in config/presets/sample/environments.inc
 */

$folders = array(
    
    //      For each folder, we must specify the link to its environment with its custom GUID :
    //      as of 2013/12/20 00:20:45 this is a concatenation of environment GUID + Path
    //      @evol 2013/12/20 00:46:15 - Tags ? Hard here : we'd need tags DB ids... unless use GUID for Tags as well ~ str_clean( title ) ?
    array(
        
        //      Hard value
        //      @see todo 2013/12/17 14:16:51 in config/presets/sample/environments.inc
        'id_environment' => 1,
        
        'path' => '/var/www/my-project/instance',
        
        'owner_group' => 'my-owner:my-group',
        'permissions' => '555',
    ),
    
    array(
        'id_environment' => 1,
        'path' => '/home/my-user/my-project/instance/db-dumps',
        'owner_group' => 'my-owner:my-group',
        'permissions' => '700',
    ),
);

//      Prepare for DB insertion
//      @evol 2013/12/20 00:12:27 - automatic handling of preprocess to read / write from / to DB
//      @see [planned] mini-ORM
foreach( $folders as $folder )
{
    $folder[ 'guid' ] = guid( $folder, array( 'parts_arr' => array( '192.168.123.123,ssh-username,project-name/www', $folder[ 'path' ])));

    //      Write to DB
    $id = insert_or_update( 'folder', $folder, array( 'unique_field_name' => 'guid' ));

    //      Feedback
    $content .= "<hr>";
    $content .= alert( "Sample folder processed - id_folder : <code>[$id]</code>" );
}


