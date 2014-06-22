<?php

/**
 *  @file
 *  Command-specific Tokens
 */


/**
 *  Replace tokens from 'raw' command,
 *  using its contextual data (environment, server, etc.),
 *  and custom values, mapped in prepare_batch_commands()
 *  
 *  @see engine/core/api_commands.inc
 *  @see prepare_batch_commands()
 */
function command_tokens_replace( $execution_step_config, $step_number = 0 )
{
    //      debug
    //krumo( 'command_tokens_replace() - $execution_step_config' );
    //krumo( $execution_step_config );
    
    //      Get all tokens
    if ( !empty( $execution_step_config[ 'command' ][ 'command' ]))
    {
        $command_str = $execution_step_config[ 'command' ][ 'command' ];
        $all_tokens = simple_tokens_extract( $execution_step_config[ 'command' ][ 'command' ]);
        
        //      debug
        //krumo( 'command_tokens_replace() - $all_tokens' );
        //krumo( $all_tokens );
        
        foreach( $all_tokens as $token_namespace => $tokens )
        {
            foreach( $tokens as $token )
            {
                $replace_str = '';
                
                //      Allow making any token "static" with a prefix (constant value for the entire batch process, across all steps)
                //      @evol 2013/09/20 13:29:50 - think about it, maybe it only makes sense for things like datestamps,
                //      so let's use custom tokens until we refine this feature.
                //$token_is_static = false;
                //if ( substr( $token_namespace, 0, 7 ) == 'static_' )
                //    $token_is_static = true;
                
                switch( $token_namespace )
                {
                    case 'folder' :
                        
                        //      Current environment's folders are in :
                        $folders = $execution_step_config[ 'environment' ][ 'folders' ];
                        
                        //      Failsafe : must have folders
                        if ( empty( $folders ))
                            continue;
                        
                        //      When there's a value, it's a tag title
                        //      (or a tag ID if it's a numeric value)
                        if ( !empty( $token[ 'value' ]))
                        {
                            //      Support numeric values (tag ID)
                            if ( is_numeric( $token[ 'value' ]))
                                $id_tag = $token[ 'value' ];
                            
                            //      Get the id from tag title (case insensitive)
                            else
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
                        $replace_str = strclean( $execution_step_config[ 'environment' ][ 'title' ], '-', true );
                        break;
                    
                    case 'db_name' :
                    case 'username' :
                    case 'password' :
                        if ( !empty( $token[ 'value' ]) && !empty( $execution_step_config[ 'environment' ][ 'data' ][ 'credentials' ]))
                            foreach( $execution_step_config[ 'environment' ][ 'data' ][ 'credentials' ] as $c )
                                if ( $c[ 'type' ] == $token[ 'value' ])
                                    $replace_str = $c[ $token_namespace ];
                        break;
                    
                    //      For chaining DB dumps creation/restoration in different steps, we need to "persist" the filename.
                    //      Process :
                    //          a) the first call (by thread) caches the value from current step env. data
                    //          b) subsequent calls (by thread) get the value
                    case 'dump_filename' :
                        $cache_namespace = "dump_filename_thread_". $execution_step_config[ 'id_thread' ];
                        $dump_filename = cache_get( $cache_namespace );
                        if ( empty( $dump_filename ))
                        {
                            $dump_filename = str_clean( $execution_step_config[ 'environment' ][ 'title' ], '-', true ) . datestamp_token();
                            cache_set( $cache_namespace, $dump_filename );
                        }
                        $replace_str = $dump_filename;
                        break;
                    
                    
                    //      Custom tokens
                    default:
                        
                        //      Mapping is done in batch command "preparation"
                        //      @see prepare_batch_commands()
                        if ( !empty( $execution_step_config[ 'custom_tokens_values_mapping' ]))
                            $replace_str = $execution_step_config[ 'custom_tokens_values_mapping' ][ $token_namespace ];
                        
                        //      Abnormal situation - minimal feedback
                        else
                            print alert( "Warning : custom token '$token_namespace' seems to have an empty value", 'warning' );
                        
                        break;
                }
                
                //      Replace token by its "rendered" value
                $command_str = str_replace( $token[ 'match' ], $replace_str, $command_str );
            }
        }
        
        //      debug
        //krumo( 'command_tokens_replace() - $command_str' );
        //krumo( $command_str );
        
        return $command_str;
    }
    
    return false;
}


