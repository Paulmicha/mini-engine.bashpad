<?php

/**
 *  @file
 *  Mini-engine helpers
 *  Persist notification messages in Session
 */


/**
 *  Set message
 *  @see http://api.drupal.org/api/drupal/includes%21bootstrap.inc/function/drupal_set_message/6
 */
function notification_set_message( $message, $type = 'status' )
{
    if ( $message )
    {
        if ( !isset( $_SESSION[ 'messages' ])) {
            $_SESSION[ 'messages' ] = array();
        }
        if ( !isset( $_SESSION[ 'messages' ][ $type ])) {
            $_SESSION[ 'messages' ][ $type ] = array();
        }
        if ( $repeat || !in_array( $message, $_SESSION[ 'messages' ][ $type ])) {
            $_SESSION[ 'messages' ][ $type ][] = $message;
        }
    }
    return isset( $_SESSION[ 'messages' ]) ? $_SESSION[ 'messages' ] : NULL;
}



/**
 *  Get messages
 *  @see http://api.drupal.org/api/drupal/includes!bootstrap.inc/function/drupal_get_messages/6
 */
function notification_get_message( $type = NULL, $clear_queue = TRUE )
{
    if ($messages = notification_set_message())
    {
        if ($type) {
            if ($clear_queue) {
                unset($_SESSION['messages'][$type]);
            }
            if (isset($messages[$type])) {
                return array($type => $messages[$type]);
            }
        }
        else {
            if ($clear_queue) {
                unset($_SESSION['messages']);
            }
            return $messages;
        }
    }
    return array();
}


