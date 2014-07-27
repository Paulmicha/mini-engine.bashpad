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


