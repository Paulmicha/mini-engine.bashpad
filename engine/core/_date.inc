<?php

/**
 *  @file
 *  Date & time utilities
 */


/**
 *  Test if a period is active
 *  
 *  @param $datetime_start int : date( "YmdHis" )
 *  @param $datetime_stop int : idem
 *  @param $get_details Boolean : set to true for getting the reason why the period may not be active
 *  @return boolean | string : in case we asked for the reason why the period may not be active,
 *      this return value can be a string - "too soon" or "too late"
 */
function datetime_period_active( $datetime_start, $datetime_stop, $get_details = false )
{
    $datetime_now = datestamp();
    
    //      This allows for getting the reason why the period may not be active
    if ( $get_details )
    {
        if ( $datetime_start > $datetime_now ) {
            return "too soon";
        }
        else if ( $datetime_now > $datetime_stop ) {
            return "too late";
        }
        else {
            return true;
        }
    }
    else {
        return ( $datetime_start < $datetime_now && $datetime_now < $datetime_stop );
    }
}



/**
 *  Quick date formatter
 */
function datestamp_format( $datestamp = '', $format = "Y-m-d @ H:i:s" )
{
    if ( empty( $datestamp ))
        $datestamp = datestamp();
    return date( $format, strtotime( $datestamp ));
}



/**
 *  Quick date formatter - from time()
 */
function timestamp_format( $time = '', $format = "Y-m-d @ H:i:s" )
{
    if ( empty( $time ))
        $time = time();
    return date( $format, $time );
}



/**
 *  datestamp() Variant - ex: 2013-07-24-13.36.41
 */
function datestamp_token( $format = "Y-m-d-H.i.s" ) {
    return date( $format, time());
}



/**
 *  Reference datestamp (BIGINT) - ex: 20130724210454
 */
function datestamp() {
    return date( dtstmpstr());
}



/**
 *  Reference datestamp - string for date
 */
function dtstmpstr() {
    return "YmdHis";
}



/**
 *  Date limite
 *  @param  int $annee_mois_date_limite : nombre formatté comme une date( "Ymd" )
 *  @param  int $heure_limite : nombre formatté comme une date( "Gi" )
 *  @return boolean
 */
function date_limite_depassee( $annee_mois_date_limite, $heure_minute_limite = 0 )
{
    $heure_minute = date( "Gi" );
    $annee_mois_date = date( "Ymd" );
    if (( intval( $annee_mois_date ) >= $annee_mois_date_limite && intval( $heure_minute ) >= $heure_minute_limite ) || intval( $annee_mois_date ) > $annee_mois_date_limite ) {
        return true;
    }
    else {
        return false;
    }
}


