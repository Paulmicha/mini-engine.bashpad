<?php

/**
 *  @file
 *  UI - nav-related
 *  @todo 2014/07/27 23:29:55 - refacto in progress (patterns / components)
 */


/**
 *  Get single menu item,
 *  handling 'active' class based on path
 */
function menu_item( $title, $path, $prefix = '', $options = array())
{
    //      Overridable defautls
    $options += array(
        'class' => '',
        'active_callback' => 'path_is_in_base_route',
        //'active_callback' => 'path_is_active',
    );
    extract( $options );
    
    //      Automatically detect if item is active
    //      @see api_routing.inc
    if ( !empty( $active_callback ) && function_exists( $active_callback ))
        if ( $active_callback( $path ))
            $class .= ' active';
    
    if ( !empty( $class ))
        $class = ' class="'. $class .'"';
    
    if ( !empty( $prefix ))
        $title = $prefix ."&nbsp;". $title;
    
    return "<li$class>". l( $title, $path ) ."</li>";
}


