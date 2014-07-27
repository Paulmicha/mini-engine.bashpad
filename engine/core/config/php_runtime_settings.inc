<?php

/**
 *  @file
 *  Mini Engine - PHP runtime settings
 */

header( 'Content-Type: text/html; charset=utf-8' );

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
setlocale( LC_ALL, $locale );
date_default_timezone_set( $timezone );

