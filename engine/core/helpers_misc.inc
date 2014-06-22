<?php

/**
 *  @file
 *  Mini-engine helper - misc.
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


