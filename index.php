<?php

/**
 *	@file
 *	Quick php mini-engine from scratch index file
 */

//      Strict IP-based access control
//      (uncomment this to see your IP)
//print '<pre>';
//print $_SERVER[ 'REMOTE_ADDR' ];
//print '</pre>';

//      Manual list of authorized IPs,
//      in addition to local IP addresses  (beginning with '192.168')
$whiltelist = array(
    
    //      localhost
    "127.0.0.1",
    
    //      Your IP ?
    //"000.000.000.000",
);

//      Only allow local IP addresses (beginning with '192.168')
//      or the ones "whitelisted" above
if ( substr( $_SERVER[ 'REMOTE_ADDR' ], 0, 7 ) != "192.168" && !in_array( $_SERVER[ 'REMOTE_ADDR' ], $whiltelist ))
    header( "HTTP/1.0 404 Not Found" );
else
    include 'engine/bootstrap.inc';


