<?php

/**
 *  @file
 *  File cache API
 *  
 *  Note : json_encode() & json_decode() produce Std Class Objects from associative arrays
 *  -> using custom "codec" functions
 *  @see http://stackoverflow.com/questions/11195692/how-to-serialize-encode-php-array-to-a-serialize-array-not-as-a-serialize-object
 *  
 *  @see safe_serialize()
 *  @see safe_unserialize()
 */


/**
 *  File cache get
 */
function filecache_get( $unique_id, $options = array() ) 
{
    //      Overridable defaults
    $options += array(
        'path' => 'engine/writeable/filecache',
        'serialize' => true,
    );
    extract( $options );
    
    $cache_id = md5( $unique_id );
    $cache_filepath = "$path/$cache_id.cache.inc";
    $data = @file_get_contents( $cache_filepath );
    
    if ( !empty( $data ))
    {
        //      Allow raw data retrieval option
        if ( !$serialize )
            return $data;
        
        //      Default : the data, already serialize-decoded
        return safe_unserialize( $data );
    }
}



/**
 *  File cache set
 */
function filecache_set( $unique_id, $data, $options = array())
{
    //      Overridable defaults
    $options += array(
        'path' => 'engine/writeable/filecache',
        'serialize' => true,
    );
    extract( $options );
    
    $cache_id = md5( $unique_id );
    $cache_filepath = "$path/$cache_id.cache.inc";
    
    if ( $serialize )
        $data = safe_serialize( $data );
    
    return file_put_contents( $cache_filepath, $data );
}



/**
 *  Delete cache entry
 */
function filecache_clear( $unique_id, $options = array())
{
    //      Overridable defaults
    $options += array(
        'path' => 'engine/writeable/filecache',
    );
    extract( $options );
    
    $cache_id = md5( $unique_id );
    $cache_filepath = "$path/$cache_id.cache.inc";
    
    return @unlink( $cache_filepath );
}


