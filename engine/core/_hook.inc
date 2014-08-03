<?php

/**
 *  @file
 *  Mini "hook" system
 *  (poorman's events)
 */


/**
 *  Invoke hook
 */
function hook( $hook, &$vars = array())
{
    global $extensions;
    foreach( $extensions as $ext => $data )
    {
        $fn_name = $ext .'_'. $hook;
        if ( function_exists( $fn_name )) {
            $fn_name( $vars );
        }
    }
}

