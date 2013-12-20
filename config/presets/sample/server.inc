<?php

/**
 *  @file
 *  Sample server
 */

$server = array(
    
    //      Note : we need a GUID to handle import/export without creating duplicates
    //      Proposition : concatenation of hostname + ssh-username
    //'id_server' => 1,
    //      @see below
    //'guid' => '192.168.123.123,ssh-username',
    
    'title' => "Server Name",
    'description' => "This kind of preset allows to quickly restore a \"clean\" mini-engine, with custom server configurations in place",
    'hostname' => '192.168.123.123',
    'data' => array(
        'ssh' => array(
            'u' => 'ssh-username',
            'p' => 'ssh-password',
            
            //      Optional
            //'k' => 'ssh-private-key',
            //'port' => 'ssh-port',
        ),
    ),
);

//      Prepare for DB insertion
//      @evol 2013/12/20 00:12:27 - automatic handling of preprocess to read / write from / to DB
//      @see [planned] mini-ORM
$server[ 'guid' ] = guid( $server, array( 'parts_arr' => array( $server[ 'hostname' ], $server[ 'data' ][ 'ssh' ][ 'u' ])));
$server[ 'data' ] = safe_serialize( $server[ 'data' ]);

//      Write to DB
$id = insert_or_update( 'server', $server, array( 'unique_field_name' => 'guid' ));

//      Feedback
$content .= "<hr>";
$content .= alert( "Sample server processed - id_server : <code>[$id]</code>" );


