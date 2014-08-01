<?php

/**
 *  @file
 *  UI - assets
 */


/**
 *  Theming helper
 *  Assets : add CSS - inline or not, in <head> (or any "space")
 */
function add_css( $css, $options = array())
{
    global $spaces;
    
    //      Overridable defaults
    $options += array(
        'in' => 'head',//  or any "space"
        'inline' => false,
        'attributes' => array(),
    );
    extract( $options );
    
    if ( !$inline ) {
        $css = '<link rel="stylesheet" href="'. url( $css ) .'"'. attributes( $attributes ) .'>';
    }
    else {
        $css = '<style'. attributes( $attributes ) .'>'. $css .'</style>';
    }
    
    $spaces[ $in ] .= $css;
}



/**
 *  Theming helper
 *  Assets : add JS - inline or not, in <head>, or in footer (or any "space")
 */
function add_js( $js, $options = array())
{
    global $spaces;
    
    //      Overridable defaults
    $options += array(
        'in' => 'page_bottom',//  or 'head', or any "space"
        'inline' => false,
        'attributes' => array(),
    );
    extract( $options );
    
    if ( !$inline ) {
        $js = '<script src="'. url( $js ) .'"'. attributes( $attributes ) .'></script>';
    }
    else {
        $js = '<script'. attributes( $attributes ) .'>'. $js .'</script>';
    }
    
    $spaces[ $in ] .= $js;
}


