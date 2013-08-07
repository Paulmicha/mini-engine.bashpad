<?php

/**
 *  @file
 *  Cache API
 *  
 *  Note : json_encode() & json_decode() produce Std Class Objects from associative arrays
 *  -> using custom "codec" functions
 *  @see http://stackoverflow.com/questions/11195692/how-to-serialize-encode-php-array-to-a-serialize-array-not-as-a-serialize-object
 *  
 *  @see install.sql
 *  @see safe_serialize()
 *  @see safe_unserialize()
 */


/**
 *  Cache get
 */
function cache_get( $namespace, $options = array() ) 
{
    //      Overridable defaults
    $options += array(
        'serialize' => true,
        'detailed' => false,
        'bypass_expire' => false,
    );
    extract( $options );
    
    $namespace = addcslashes( check_plain( $namespace ), '"' );
    $cache = db_fetch( "SELECT * FROM `cache` WHERE `namespace` LIKE \"$namespace\" ORDER BY `id_cache` DESC LIMIT 0,1" );
    
    if ( !empty( $cache ))
    {
        //      Check if value expired (check does not apply when 
        if ( !$bypass_expire && !empty( $cache[ 'datestamp_expire' ]))
        {
            $datestamp_now = datestamp();
            if ( $datestamp_now > $cache[ 'datestamp_expire' ])
                return false;
        }
        
        //      Details retrieval option
        if ( $detailed )
            return $cache;
        
        //      Allow raw data retrieval option
        if ( !$serialize )
            return $cache[ 'data' ];
        
        //      Default : the data, already serialize-decoded
        //return unserialize( $cache[ 'data' ]);
        return safe_unserialize( $cache[ 'data' ]);
    }
    
    return false;
}



/**
 *  Cache set
 *  update 2013/03/27 11:32:00 - never expires by default.
 */
function cache_set( $namespace, $data, $options = array())
{
    //      Overridable defaults
    $options += array(
        'serialize' => true,
        'datestamp' => datestamp(),
        //'expire' => "+1 week",
    );
    extract( $options );
    
    //if ( !empty( $data ) && $serialize )
    if ( $serialize )
        //$data = serialize( $data );
        $data = safe_serialize( $data );
    
    $values = array(
        'namespace' => $namespace,
        'data' => $data,
        'datestamp' => $datestamp,
    );
    
    if ( empty( $datestamp_expire ) && !empty( $expire ))
        $datestamp_expire = date( dtstmpstr(), strtotime( $expire ));
    if ( !empty( $datestamp_expire ))
        $values[ 'datestamp_expire' ] = $datestamp_expire;
    if ( !empty( $title ))
        $values[ 'title' ] = $title;
    
    if ( mandatory_values_check( $values, array( 'namespace', 'data' )))
        return insert_or_update( 'cache', $values, array( 'unique_field_name' => 'namespace' ));
    
    return false;
}



/**
 *  Delete cache entry
 */
function cache_clear( $namespace )
{
    $namespace = addcslashes( check_plain( $namespace ), '"' );
    return db_query( "DELETE FROM `cache` WHERE `namespace` LIKE \"$namespace\"" );
}



/**
 *  Make link to clear cache
 */
function cache_clear_link( $namespace, $options = array())
{
    //      Overridable defaults
    $options += array(
        'title' => "<i class='icon-trash'></i>&nbsp;Clear cache <code>$namespace</code>",
        'class' => array(),  //  also accepts strings
        'ajax' => true,
    );
    extract( $options );
    
    $classes = '';
    if ( !empty( $class ))
    {
        if ( is_array( $class ))
            $classes = " class='". join( ' ', $class ) ."'";
        else
            $classes = " class='". $class ."'";
    }
    
    return "<a". $classes ." href='". base_path() ."system/cache-clear/". addcslashes( check_plain( $namespace ), "'" ) ."' target='_blank'>$title</a>";
}


