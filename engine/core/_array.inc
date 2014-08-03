<?php

/**
 *  @file
 *  Array utilities
 */


/**
 *  Appends all values from 2nd array to the 1st array.
 *  Combine those 2 arrays into one, containing all the values from both.
 *  Doesn't care about duplicates. Nor keys.
 */
function array_append_values( $arr, $values )
{
    if ( !empty( $values ) && is_array( $values ))
        foreach( $values as $val )
            $arr[] = $val;
    
    return $arr;
}


/**
 *  Recursively merge arrays, overriding 1st array's values when keys collide
 *  @see http://www.php.net/manual/fr/function.array-merge-recursive.php#102379
 */
function array_merge_recursive_overwrite( $arr1, $arr2 )
{
    foreach( $arr2 as $key => $value )
    {
        if ( array_key_exists( $key, $arr1 ) && is_array( $value )) {
            $arr1[ $key ] = array_merge_recursive_overwrite( $arr1[ $key ], $arr2[ $key ]);
        }
        else {
            $arr1[ $key ] = $value;
        }
    }
    return $arr1;
}

