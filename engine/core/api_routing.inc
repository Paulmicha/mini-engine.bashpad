<?php

/**
 *  @file
 *  Routing API
 *  
 *  These are just helpers, the 'routing' process happens in :
 *  @see engine/core/http_request_engine.inc
 */


/**
 *  Utility : get URL argument "à la Drupal"
 *  
 *  Guarantees that calling "arg( 1 )" always returns
 *  the same relevant contextual value for any kind of HTTP calls (normal, 'rest' or 'iframe')
 *  
 *  @see engine/core/http_request_engine.inc
 */
function arg( $i )
{
    if ( !empty( $_REQUEST[ 'q' ]))
    {
        $request_arr = explode( '/', $_REQUEST[ 'q' ]);
        switch ( $request_arr[ 0 ])
        {
            case 'rest':
            case 'iframe':
                array_shift( $request_arr );
                return $request_arr[ $i ];
            
            default:
                return $request_arr[ $i ];
        }
    }
    return null;
}



/**
 *  Auto-prefix paths
 *  @param $path (String) no trailing or starting slash
 */
function url( $path, $options = array())
{
    //      Alread absolute URLs must not be touched
    if ( substr( $path, 0, 7 ) == 'http://' || substr( $path, 0, 8 ) == 'https://' )
        return $path;
    
    $options += array(
        'absolute' => false,
    );
    
    if ( $options[ 'absolute' ])
    {
        $url = 'http';
        if ( $_SERVER[ "HTTPS" ] == "on" ) {
            $url .= "s";
        }
        $url .= "://";
        if ( $_SERVER[ "SERVER_PORT" ] != "80") {
            $url .= $_SERVER[ "SERVER_NAME" ] .":". $_SERVER[ "SERVER_PORT" ];
        } else {
            $url .= $_SERVER[ "SERVER_NAME" ];
        }
        //      Note : base_path() always MUST start and end with "/"
        return $url . base_path() . $path;
        //return $url . $path;
    }
    else
        return base_path() . $path;
}



/**
 *  Link helper
 *  
 *  @param String $text : link text
 *  @param String $href : link URL
 *  @param Array $options : url() $options
 *  @return String rendered HTML
 */
function l( $text, $href, $options = array())
{
    $options += array(
        'absolute' => true,
        'external' => false,
        'attributes' => array(),
    );
    
    if ( $options[ 'external' ])
    {
        $options[ 'attributes' ][ 'target' ] = "_blank";
        $text = '<i class="icon-external-link"></i>&nbsp;'. $text;
    }
    
    $attributes = '';
    if ( !empty( $options[ 'attributes' ]))
    {
        $attributes = attributes( $options[ 'attributes' ]);
        unset( $options[ 'attributes' ]);
    }
    
    $href = url( $href, $options );
    $href = addcslashes( $href, "'" );
    
    return "<a$attributes href='$href'>$text</a>";
}



/**
 *  Get current page URL
 */
function get_current_url( $options = array())
{
    $options += array(
        'no_vars' => false,
        'absolute' => true,
    );
    
    if ( $options[ 'absolute' ])
    {
        $url = 'http';
        if ( $_SERVER[ "HTTPS" ] == "on" ) {
            $url .= "s";
        }
        $url .= "://";
        if ( $_SERVER[ "SERVER_PORT" ] != "80") {
            $url .= $_SERVER[ "SERVER_NAME" ] .":". $_SERVER[ "SERVER_PORT" ] . $_SERVER[ "REQUEST_URI" ];
        } else {
            $url .= $_SERVER[ "SERVER_NAME" ] . $_SERVER[ "REQUEST_URI" ];
        }
    }
    else {
        $url = $_SERVER[ "REQUEST_URI" ];
    }
    if ( $options[ 'no_vars' ]) {
        $url = strtok( $url, '?' );
    }
    return $url;
}


/**
 *  Utility : get current path
 */
function get_current_path( $options = array())
{
    //      Home
    if ( !isset( $_REQUEST[ 'q' ]) || empty( $_REQUEST[ 'q' ]))
        return base_path();
    
    //      The rest
    if ( $options[ 'with_base_path' ])
        return url( $_REQUEST[ 'q' ]);
    else
        return $_REQUEST[ 'q' ];
}



/**
 *  Utility : is this URL the currently active path ?
 */
function path_is_active( $url )
{
    //      Home
    if (( $url == base_path() || empty( $url )) && empty( $_GET ))
        return true;
    
    //      The rest
    if ( !empty( $_GET[ 'q' ]))
    {
        $request_str = $_GET[ 'q' ];
        $clean_path_str = join( '/', explode( '/', strtok( $url, '?' )));
        if ( $request_str == $clean_path_str )
            return true;
    }
    return false;
}



/**
 *  Utility : is the current path inside the chain of "base routes" ?
 */
function path_is_in_base_route( $base_route )
{
    //      This one ?
    if ( path_is_active( $base_route ))
        return true;
    
    //      Checking up to 20 args
    $path_check_arr = array();
    for ( $i = 0; $i < 20; $i ++ )
    {
        $arg = arg( $i );
        if ( $arg !== 0 && empty( $arg ))
            break;
        $path_check_arr[] = $arg;
    }
    
    while( array_pop( $path_check_arr ))
        if ( !empty( $path_check_arr ) && $base_route == join( '/', $path_check_arr ))
            return true;
    
    return false;
}



/**
 *  Drupal rip-off
 *  @see http://api.drupal.org/api/drupal/core%21includes%21bootstrap.inc/function/request_uri/8
 */
function request_uri( $omit_query_string = FALSE )
{
  if (isset($_SERVER['REQUEST_URI'])) {
    $uri = $_SERVER['REQUEST_URI'];
  }
  else {
    if (isset($_SERVER['argv'][0])) {
      $uri = $_SERVER['SCRIPT_NAME'] . '?' . $_SERVER['argv'][0];
    }
    elseif (isset($_SERVER['QUERY_STRING'])) {
      $uri = $_SERVER['SCRIPT_NAME'] . '?' . $_SERVER['QUERY_STRING'];
    }
    else {
      $uri = $_SERVER['SCRIPT_NAME'];
    }
  }
  // Prevent multiple slashes to avoid cross site requests via the Form API.
  $uri = '/' . ltrim($uri, '/');

  return $omit_query_string ? strtok($uri, '?') : $uri;
}



/**
 *  Utility : make a "reload current page" link
 */
function reload_link( $label = 'reload' )
{
    //$current_url = get_current_path( array( 'with_base_path' => true ));
    $current_url = get_current_path();
    
    if ( !empty( $_GET ))
    {
        if ( !empty( $_GET[ 'q' ]))
            unset( $_GET[ 'q' ]);
        if ( !empty( $_GET ))
            $current_url .= "?". http_build_query( $_GET );
    }
    
    return "<a href='". url( $current_url ) ."'><i class='icon-repeat'></i>&nbsp;$label </a>";
}


