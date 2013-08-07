<?php

/**
 *  @file
 *  Server - status
 *  @path /sytem/server/1/status
 */

//      Init
$head_page_title = "Server status : $server_title";

//      Simple test
$ssh = ssh( $arg2 );

if ( !$ssh )
    $content .= alert( "SSH Connection failed.", 'error' );
else
{
    $content .= alert( "SSH Connection successful.", 'success' );
    
    //      List files in home dir
    $std_out = $ssh->exec( 'ls -la' );
    $content .= '<pre>';
    $content .= "'ls -la' = $std_out";
    $content .= '</pre>';
}


