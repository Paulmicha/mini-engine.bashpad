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
 *  @see http://stackoverflow.com/questions/2942385/php-code-execution-time
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
    $return .= round( $microtime_elapsed, 3 ) . " s";
    return $return;
}


