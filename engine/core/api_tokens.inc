<?php

/**
 *  @file
 *  Tokens API
 */


/**
 *  Replace tokens from 'raw' command,
 *  using its contextual data (environment, server, etc.),
 *  and custom values, mapped in prepare_batch_commands()
 *  
 *  @see engine/core/api_commands.inc
 *  @see prepare_batch_commands()
 */
function command_tokens_replace( $execution_step_config )
{
    //      debug
    //krumo( $execution_step_config );
    
    //      Get all tokens
    if ( !empty( $execution_step_config[ 'command' ][ 'command' ]))
    {
        $command_str = $execution_step_config[ 'command' ][ 'command' ];
        $auto_tokens = get_automatic_tokens();
        $all_tokens = simple_tokens_extract( $execution_step_config[ 'command' ][ 'command' ]);
        
        //      debug
        //krumo( $all_tokens );
        
        foreach( $all_tokens as $token_namespace => $tokens )
        {
            //      "Automatic" tokens : the ones we can guess from context (folder, environment, server)
            if ( in_array( $token_namespace, $auto_tokens ))
            {
                foreach( $tokens as $token )
                {
                    $replace_str = '';
                    
                    switch( $token_namespace )
                    {
                        case 'folder' :
                            
                            //      Current environment's folders are in :
                            $folders = $execution_step_config[ 'environment' ][ 'folders' ];
                            
                            //      Failsafe : must have folders
                            if ( empty( $folders ))
                                continue;
                            
                            //      When there's a value, it's a tag title
                            if ( !empty( $token[ 'value' ]))
                            {
                                //      Get the id from tag title (case insensitive)
                                $id_tag = get_tag_by_title( $token[ 'value' ]);
                                
                                //      Get the last folder in this environment that has this tag
                                if ( !empty( $id_tag ))
                                    foreach( $folders as $f )
                                        if ( in_array( $id_tag, $f[ 'tags' ]))
                                            $replace_str = $f[ 'path' ];
                            }
                            
                            //      Else it's the first folder we find in this environment that will do (if any)
                            else
                            {
                                $f = array_shift( $folders );
                                $replace_str = $f[ 'path' ];
                            }
                            
                            break;
                        
                        case 'datestamp' :
                            $replace_str = datestamp_token();
                            break;
                        
                        case 'env' :
                            $replace_str = $execution_step_config[ 'environment' ][ 'title' ];
                            break;
                        
                        case 'db_name' :
                        case 'username' :
                        case 'password' :
                            if ( !empty( $token[ 'value' ]) && !empty( $execution_step_config[ 'environment' ][ 'data' ][ 'credentials' ]))
                                foreach( $execution_step_config[ 'environment' ][ 'data' ][ 'credentials' ] as $c )
                                    if ( $c[ 'type' ] == $token[ 'value' ])
                                        $replace_str = $c[ $token_namespace ];
                            break;
                    }
                    
                    //      Finally, replace with its path
                    $command_str = str_replace( $token[ 'match' ], $replace_str, $command_str );
                }
            }
            
            //      Custom tokens : values must be in $execution_step_config data
            else
            {
                //      @todo 2013/08/02 17:54:17
            }
        }
        
        return $command_str;
    }
    
    return false;
}



/**
 *  Get custom tokens
 *  
 *  @example [token_namespace]
 *  @example [token_namespace:qsdqsd123]
 *  
 *  @return Array: array( 'token_namespace' => array( 0 => array( 'value' => '', 'match' => '[type_token]' )))
 *  @return Array: array( 'token_namespace' => array( 0 => array( 'value' => 'qsdqsd123', 'match' => '[type_token:qsdqsd123]' )))
 */
function simple_tokens_extract( $str )
{
    $return = array();
    if ( !empty( $str ))
    {
        $matches = array();
        $feedback = preg_match_all( '`\[([^:]+):?([^\]]+)?\]`i', $str, $matches, PREG_SET_ORDER );
        
        //      debug
        //krumo( $matches );
        
        if ( $feedback && !empty( $matches ))
            foreach( $matches as $i => $match )
                $return[ $match[ 1 ]][] = array(
                    'value' => $match[ 2 ],
                    'match' => $match[ 0 ]
                );
    }
    return $return;
}



/**
 *  List of "automatic" tokens
 */
function get_automatic_tokens()
{
    //      Whitelist
    return array(
        'folder',
        'datestamp',
        'env',
        
        //      Those have args
        'username',
        'password',
        'dbname',
        
        //      @evol 2013/08/02 17:49:43 - support for tokens with values - e.g. [environment:title] ?
        //'environment' => array(
        //    'title',
        //    'hostname',
        //),
    );
}


