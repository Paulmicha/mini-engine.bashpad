<?php

/**
 *  @file
 *  Database API - config & static connection
 */


/**
 *  Static PDO DB Connection
 *  (poorman's singleton)
 */
function db_pdo()
{
    static $pdo;
    global $extensions;
    
    if ( !$pdo[ $extensions[ 'db' ][ 'active' ]])
    {
        extract( get_db_config( $extensions[ 'db' ][ 'active' ]));
        
        $pdo[ $extensions[ 'db' ][ 'active' ]] = new PDO(
            $driver
                .':host='. $host
                . ( !empty( $port ) ? ';port='. $port : '' )
                .';dbname='. $db_name,
            $user,
            $pass
        );
        
        if ( !empty( $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_attr' ]))
            foreach( $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_attr' ] as $attr => $val )
                $pdo[ $extensions[ 'db' ][ 'active' ]]->setAttribute( $attr, $val );
        
        $pdo[ $extensions[ 'db' ][ 'active' ]]->exec( 'SET CHARACTER SET utf8' );
    }
    
    return $pdo[ $extensions[ 'db' ][ 'active' ]];
}



/**
 *  DB settings 'getter' function
 *  + defaults
 */
function get_db_config( $db = 'default' )
{
    global $extensions;
    
    //      Default driver : mysql
    if ( empty( $extensions[ 'db' ][ 'settings' ][ $db ][ 'driver' ])) {
        $extensions[ 'db' ][ 'settings' ][ $db ][ 'driver' ] = 'mysql';
    }
    
    //      Default PDO attributes
    if ( empty( $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_attr' ]))
    {
        $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_attr' ] = array();
        $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_attr' ][ PDO::ATTR_ERRMODE ] = PDO::ERRMODE_EXCEPTION;
        
        if ( 'mysql' == $extensions[ 'db' ][ 'settings' ][ $db ][ 'driver' ]) {
            $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_attr' ][ PDO::MYSQL_ATTR_USE_BUFFERED_QUERY ] = true;
        }
    }
    
    //      Default PDO param handling
    if ( empty( $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_named_params' ])) {
        $extensions[ 'db' ][ 'settings' ][ $db ][ 'pdo_named_params' ] = false;
    }
    
    //      Default host : localhost
    if ( empty( $extensions[ 'db' ][ 'settings' ][ $db ][ 'host' ])) {
        $extensions[ 'db' ][ 'settings' ][ $db ][ 'host' ] = 'localhost';
    }
    
    return $extensions[ 'db' ][ 'settings' ][ $db ];
}


