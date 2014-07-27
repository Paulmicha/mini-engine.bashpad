<?php

/**
 *  @file
 *  Debug utilities
 */


/**
 *  Debug helper (pretty print with krumo)
 *  -> wrap in output buffer
 *  @requires krumo
 */
function kpr( $object )
{
    ob_start();
    krumo( $object );
    $output = ob_get_contents();
    ob_end_clean();
    return $output;
}



/**
 *  Execution time helper
 *  @see    http://stackoverflow.com/questions/2942385/php-code-execution-time
 */
StopWatch::start();
class StopWatch
{
    private static $total;
    public static function start() {
        self::$total = microtime(true);
    }
    public static function elapsed() {
        return microtime(true) - self::$total;
    }
}


/**
 *  Statistics / Performance measure
 *  Calculate duration of execution : end
 */
function get_system_exec_time()
{
    $return = '';
    $return .= "execution time : ";
    
    $microtime_elapsed = StopWatch::elapsed();
    //if ( $microtime_elapsed >= 1 )
        //$return .= round( $microtime_elapsed, 2 ) . " s";
        $return .= round( $microtime_elapsed, 3 ) . " s";
    //else
        //$return .= ( round( $microtime_elapsed, 5 ) * 1000 ) . " ms";
    
    //$ru = getrusage();
    //$return .= "This process used " . rutime( $ru, $GLOBALS[ 'rustart' ], "utime" ) . " ms for its computations\n";
    //$return .= "It spent " . rutime( $ru, $GLOBALS[ 'rustart' ], "stime" ) . " ms in system calls\n";
    
    //$return .= "execution time : ";
    //$return .= rutime( $ru, $GLOBALS[ 'rustart' ], "utime" ) . " ms";
    //$return .= "'system calls' time : ";
    //$return .= rutime( $ru, $GLOBALS[ 'rustart' ], "stime" ) . " ms";
    
    return $return;
}


