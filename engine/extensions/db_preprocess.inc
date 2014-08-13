<?php

/**
 *  @file
 *  Database API - preprocess render
 */


/**
 *  Implements hook_preprocess_render()
 *  Display a warning if DB credentials are missing from settings
 */
function db_preprocess_render()
{
    global $extensions;
    $db_config = get_db_config( $extensions[ 'db' ][ 'active' ]);
    
    if ( !mandatory_values_check( $db_config, array( 'user', 'host' )))
    {
        global $content;
        $content = alert( "Missing database connection settings", 'error' )
            . alert( "Go edit <code>config/settings.inc</code> you must." )
            . $content;
    }
}
