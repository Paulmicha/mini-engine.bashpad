<?php

/**
 *  @file
 *  PhpSecLib helpers
 *  @see http://phpseclib.sourceforge.net/index.html
 */


/**
 *  SSH connections
 */
function ssh( $server )
{
    static $connections;
    
    //      Polymorphism : String (hostname), or Integer (id_server), or Array : result of get_host_data( $server );
    if ( !is_array( $server ))
        $server = get_host_data( $server );
    
    if ( empty( $connections[ $server[ 'hostname' ]]) && !empty( $server[ 'ssh' ][ 'u' ]) && !empty( $server[ 'ssh' ][ 'p' ]))
    {
        $ssh = new Net_SSH2( $server[ 'hostname' ]);
        if ( !$ssh->login( $server[ 'ssh' ][ 'u' ], $server[ 'ssh' ][ 'p' ])) {
            return false;
        }
        $connections[ $server[ 'hostname' ]] = $ssh;
    }
    return $connections[ $server[ 'hostname' ]];
}



/**
 *  SFTP connections
 */
function sftp( $server )
{
    static $connections;
    
    //      Polymorphism : String (hostname), or Integer (id_server), or Array : result of get_host_data( $server );
    if ( !is_array( $server ))
        $server = get_host_data( $server );
    
    if ( empty( $connections[ $server[ 'hostname' ]]) && !empty( $server[ 'ssh' ][ 'u' ]) && !empty( $server[ 'ssh' ][ 'p' ]))
    {
        $sftp = new Net_SFTP( $server[ 'hostname' ]);
        if ( !$sftp->login( $server[ 'ssh' ][ 'u' ], $server[ 'ssh' ][ 'p' ])) {
            return false;
        }
        $connections[ $server[ 'hostname' ]] = $sftp;
    }
    return $connections[ $server[ 'hostname' ]];
}



/**
 *  Get host data
 */
function get_host_data( $server )
{
    $data = array();
    
    //      Polymorphism : String (hostname) or Integer (id_server)
    if ( !empty( $server ))
    {
        $db_row = array();
        if ( is_numeric( $server ))
            $db_row = db_fetch( "SELECT * FROM `server` WHERE `id_server` LIKE ?", array( $server ));
        elseif ( is_string( $server ))
            $db_row = db_fetch( "SELECT * FROM `server` WHERE `hostname` LIKE ?", array( $server ));
        
        if ( !empty( $db_row ))
        {
            if ( !empty( $db_row[ 'data' ]))
                $data = safe_unserialize( $db_row[ 'data' ]);
             
             $data += array(
                'hostname' => $db_row[ 'hostname' ],
                'id_server' => $db_row[ 'id_server' ],
                'title' => $db_row[ 'title' ],
                'description' => $db_row[ 'description' ],
            );
        }
    }
    
    return $data;
}


