<?php

/**
 *  @file
 *  Mini Engine - PHP runtime settings
 */

//      Errors
if ( $enable_debug ) {
    error_reporting( E_ALL );
    //error_reporting( E_ERROR | E_WARNING | E_PARSE | E_COMPILE_ERROR );
}
else {
    error_reporting( 0 );
}

//      Encoding for multibyt functions
mb_internal_encoding( "UTF-8" );

//      Sessions
if ( $enable_session || $enable_notification )
{
    //		Fix IE sessions inside iframes
    //      @see http://stackoverflow.com/questions/5257983/what-is-headerp3p-cp-cao-psa-our
    header( 'P3P: CP="CAO PSA OUR"' );
    header( 'Content-Type: text/html; charset=utf-8' );
    
    session_start();
}

//      Local settings
setlocale( LC_ALL, 'en_EN' );
//setlocale( LC_ALL, 'fr_FR' );
date_default_timezone_set( 'UTC' );
//date_default_timezone_set( 'Europe/Paris' );


