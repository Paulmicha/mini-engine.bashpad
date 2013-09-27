<?php

/**
 *  @file
 *  DBLog API
 *  (Drupal 'watchdog' rip-off)
 */


/**
 *  Set dblog with link in 'has_dblog' table
 */
function linked_dblog( $id, $table, $message, $options = array())
{
    return db_insert( 'has_dblog', array(
        'id' => $id,
        'id_dblog' => dblog( $table, $message, $options ),
        'db_table' => $table,
    ));
}



/**
 *  DBLog
 */
function dblog( $type, $message, $options = array())
{
    //      Overridable defaults
    $options += array(
        //'severity' => DBLOG_NOTICE,
        'severity' => DBLOG_INFO,
        //'link' => NULL,
        //'data' => array(),
    );
    extract( $options );
    
    //      Prepare the fields to be logged
    $log_entry = array(
        'type' => $type,
        'message' => $message,
        'severity' => $severity,
        'request_uri' => request_uri(),
        'referer' => isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '',
        'hostname' => ip_address(),
        
        //      Request time isn't accurate for long processes, use time() instead.
        'timestamp' => time(),
        
        //      Custom mini-engine format
        'datestamp' => datestamp(),
    );
    
    if ( !empty( $data ))
        $log_entry[ 'data' ] = $data;
    
    return db_insert( 'dblog', array(
        'type' => substr($log_entry['type'], 0, 64),
        'message' => $log_entry['message'],
        'data' => @safe_serialize($log_entry['data']),
        'severity' => $log_entry['severity'],
        'location' => $log_entry['request_uri'],
        'referer' => $log_entry['referer'],
        'hostname' => substr($log_entry['hostname'], 0, 128),
        'timestamp' => $log_entry['timestamp'],
        'datestamp' => $log_entry['datestamp'],
    ));
}


/**
 *  Get latest DBLog entries
 */
function latest_dblog_entries( $options = array())
{
    //      Overridable defaults
    $options += array(
        //'type' => null,
        //'severity' => null,
        'pager' => array(
            'start' => 0,
            'nb_per_page' => 3000,
        ),
    );
    extract( $options );
    
    //      Polymorphism in fields declaration
    //          string : the label, or :
    //          array : 'render' (function), 'label'
    $fields = array(
        'id_dblog' => "ID",
        'timestamp' => array(
            'label' => "Timestamp",
            'render' => 'timestamp_format',
        ),
        'message' => "Message",
        'severity' => array(
            'label' => "Severity",
            'render' => 'read_dblog_severity_level',
        ),
        'data' => array(
            'label' => "Vars",
            'render' => 'read_dblog_vars',
        ),
        'location' => "Location",
        'referer' => "URL of referring page",
        'hostname' => "IP",
    );
    
    //      Query to fetch all entries
    $results_arr = db_fetchall( "SELECT ". join( ', ', array_keys( $fields )) ." FROM dblog ORDER BY id_dblog DESC LIMIT ". $pager[ 'start' ] .",". $pager[ 'nb_per_page' ]);
    
    //      Rendering results in a table
    //      -> table header
    $table_header = array();
    foreach( $fields as $f )
        if ( is_array( $f ))
            $table_header[] = $f[ 'label' ];
        else
            $table_header[] = $f;
    
    //      -> table rows
    $table_rows = array();
    if ( !empty( $results_arr ))
        foreach( $results_arr as $i => $r )
            foreach( $fields as $field_name => $f )
                if ( is_array( $f ))
                    $table_rows[ $i ][] = $f[ 'render' ]( $r[ $field_name ]);
                else
                    $table_rows[ $i ][] = $r[ $field_name ];
    
    return datatable( $table_rows, array( 'header' => $table_header ));
}



/**
 *  Helper : human-readable level translation
 */
function read_dblog_vars( $safe_serialized_vars )
{
    $vars = safe_unserialize( $safe_serialized_vars );
    $vars_rendered = print_r( $vars, 1 );
    if ( !empty( $vars_rendered ))
        return kpr( $vars );
    return "";
}



/**
 *  Helper : human-readable level translation
 */
function read_dblog_severity_level( $level_int )
{
    $levels = dblog_severity_levels();
    return $levels[ $level_int ];
}



/**
 *  Helper : human-readable levels
 */
function dblog_severity_levels()
{
    return array(
        DBLOG_EMERGENCY => 'emergency',
        DBLOG_ALERT => 'alert',
        DBLOG_CRITICAL => 'critical',
        DBLOG_ERROR => 'error',
        DBLOG_WARNING => 'warning',
        DBLOG_NOTICE => 'notice',
        DBLOG_INFO => 'info',
        DBLOG_DEBUG => 'debug',
    );
}



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
 * @defgroup logging_severity_levels Logging severity levels
 * @{
 * Logging severity levels as defined in RFC 3164.
 *
 * The DBLOG_* constant definitions correspond to the logging severity levels
 * defined in RFC 3164, section 4.1.1. PHP supplies predefined LOG_* constants
 * for use in the syslog() function, but their values on Windows builds do not
 * correspond to RFC 3164. The associated PHP bug report was closed with the
 * comment, "And it's also not a bug, as Windows just have less log levels,"
 * and "So the behavior you're seeing is perfectly normal."
 *
 * @see http://www.faqs.org/rfcs/rfc3164.html
 * @see http://bugs.php.net/bug.php?id=18090
 * @see http://php.net/manual/function.syslog.php
 * @see http://php.net/manual/network.constants.php
 * @see dblog()
 * @see dblog_severity_levels()
 */

/**
 * Log message severity -- Emergency: system is unusable.
 */
define('DBLOG_EMERGENCY', 0);

/**
 * Log message severity -- Alert: action must be taken immediately.
 */
define('DBLOG_ALERT', 1);

/**
 * Log message severity -- Critical conditions.
 */
define('DBLOG_CRITICAL', 2);

/**
 * Log message severity -- Error conditions.
 */
define('DBLOG_ERROR', 3);

/**
 * Log message severity -- Warning conditions.
 */
define('DBLOG_WARNING', 4);

/**
 * Log message severity -- Normal but significant conditions.
 */
define('DBLOG_NOTICE', 5);

/**
 * Log message severity -- Informational messages.
 */
define('DBLOG_INFO', 6);

/**
 * Log message severity -- Debug-level messages.
 */
define('DBLOG_DEBUG', 7);

/**
 * @} End of "defgroup logging_severity_levels".
 */


