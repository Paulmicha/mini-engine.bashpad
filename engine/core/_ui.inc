<?php

/**
 *  @file
 *  UI (theming helpers)
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
 *  Render tpl
 *  Warning : NOT checking if tpl file exists
 */
function render_tpl( $tpl_filepath, $options = array())
{
    //      Fugly global scope mayhem :3
    //      (to access any global var from within any tpl to render)
    extract( $GLOBALS );
    
    //      Overridable defaults
    $options += array(
        'prefix' => '',
        'suffix' => '',
    );
    
    //      Using output buffer
    ob_start();
    include $tpl_filepath;
    $markup = ob_get_contents();
    ob_end_clean();
    
    //      Allow (optional) prefix and/or suffix insertion
    $markup = $options[ 'prefix' ] . $markup;
    $markup .= $options[ 'suffix' ];
    
    return $markup;
}


