<?php

/**
 *  @file
 *  Mini Engine - PHP runtime settings
 */

//		Support old IE sessions inside iframes
//      @see http://stackoverflow.com/questions/5257983/what-is-headerp3p-cp-cao-psa-our
if ( arg( 0 ) == 'iframe' ) {
    header( 'P3P: CP="CAO PSA OUR"' );
}

header( 'Content-Type: text/html; charset=utf-8' );

if ( !empty( $extensions[ 'session' ][ 'enabled' ])) {
    session_start();
}

//      Display PHP Errors
if ( $enable_debug ) {
    error_reporting( E_ALL );
}
else {
    error_reporting( 0 );
}

//      UTF-8 for multibyte functions (always)
mb_internal_encoding( "UTF-8" );

//      Date & time settings
setlocale( LC_ALL, $metadata[ 'locale' ] );
date_default_timezone_set( $metadata[ 'timezone' ] );

