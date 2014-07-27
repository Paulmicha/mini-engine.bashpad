<?php

/**
 *  @file
 *  Network-related
 */


/**
 *  Helper : get user's IP
 *  @see http://stackoverflow.com/questions/13646690/how-to-get-real-ip-from-visitor
 */
function ip_address()
{
    $client  = @$_SERVER[ 'HTTP_CLIENT_IP' ];
    $forward = @$_SERVER[ 'HTTP_X_FORWARDED_FOR' ];
    $remote  = $_SERVER[ 'REMOTE_ADDR' ];
    if ( filter_var( $client, FILTER_VALIDATE_IP )) {
        $ip = $client;
    }
    elseif ( filter_var( $forward, FILTER_VALIDATE_IP )) {
        $ip = $forward;
    }
    else {
        $ip = $remote;
    }
    return $ip;
}



/**
 *  404
 */
function send_404()
{
    header( $_SERVER[ "SERVER_PROTOCOL" ] ." 404 Not Found" );
    header( "Status: 404 Not Found" );
    $_SERVER[ 'REDIRECT_STATUS' ] = 404;
    $GLOBALS[ 'content_tpl' ] = $GLOBALS[ 'theme_path' ] .'/404.tpl.php';
}



/**
 *  JSON response
 */
function send_json( $data = null )
{
    header( 'Content-Type: application/json; charset=utf-8' );
    
    if ( !empty( $data )) {
        echo json_encode( $data );
    }
}



/**
 *  XML response
 */
function send_xml() {
    header( "Content-type: text/xml; charset=utf-8" );
}



/**
 *  Plain TXT response
 */
function send_txt() {
    header( 'Content-Type: text/plain; charset=utf-8' );
}


