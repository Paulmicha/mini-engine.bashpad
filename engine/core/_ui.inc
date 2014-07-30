<?php

/**
 *  @file
 *  UI (theming helpers)
 *  @todo 2014/07/27 23:29:55 - refacto in progress (patterns / components)
 */


/**
 *  Render html attributes
 */
function attributes( $attributes )
{
    $return = '';
    
    if ( !empty( $attributes ) && is_array( $attributes ))
    {
        foreach( $attributes as $attribute => $value )
        {
            if ( !empty( $value ) && !empty( $attribute ) && !is_numeric( $attribute ))
            {
                $value = addcslashes( $value, "'" );
                $return .= " $attribute='$value'";
            }
        }
    }
    
    return $return;
}



/**
 *  Theming helper
 *  Assets : add CSS - inline or not, in <head>
 */
function add_css( $css, $options = array())
{
    //      Overridable defaults
    $options += array(
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
    
    $GLOBALS[ 'head_extra_markup' ] .= $css;
}



/**
 *  Theming helper
 *  Assets : add JS - inline or not, in <head>, or in footer
 */
function add_js( $js, $options = array())
{
    //      Overridable defaults
    $options += array(
        'in' => 'footer',// or 'head'
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
    
    if ( $in == 'footer' ) {
        $GLOBALS[ 'footer_extra_js' ] .= $js;
    }
    else {
        $GLOBALS[ 'head_extra_markup' ] .= $js;
    }
}


