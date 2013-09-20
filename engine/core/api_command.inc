<?php

/**
 *  @file
 *  Command API
 */


/**
 *  Batch command helper
 *  
 *  Tasks :
 *      a) Prepare commands configuration form submission data
 *      b) Replace tokens in commands
 *      c) Optimization : "Merge" consecutive steps happening in the same server (to make fewer SSH connections)
 *      d) Store into 'thread' data
 *  
 *  @see engine/core/api_tokens.inc
 *  @see command_tokens_replace()
 */
function prepare_batch_commands( $batch )
{
    $id_batch = $batch[ 'id_batch' ];
    
    //      a) Configuration form submission handler
    //      Custom tokens values mapping
    $execution_steps_config = array();
    if ( !empty( $_REQUEST[ 'batch_'. $id_batch .'_init_requested' ]))
        foreach( $_REQUEST as $key => $custom_value )
            if ( substr( $name, 0, 12 ) == 'custom-token' )
            {
                $token_namespace = str_replace( 'custom-token-', '', $name );
                $execution_steps_config[ $step_number ][ 'custom_tokens_values_mapping' ][ $token_namespace ] = $custom_value;
            }
    
    //      b) Process tokens
    $grouped_step_number = 0;
    $id_server_previous_command = 0;
    $grouped_rendered_commands = array();
    foreach( $batch[ 'steps' ] as $step_number => $step )
    {
        //      We need to have 'id_thread' in command_tokens_replace()
        $execution_steps_config[ $step_number ][ 'id_thread' ] = $batch[ 'id_thread' ];
        
        //      Copy entire steps' contents to allow "auto-matches" in tokens
        $execution_steps_config[ $step_number ] = $step;
        $execution_steps_config[ $step_number ][ 'rendered_command' ] = command_tokens_replace( $execution_steps_config[ $step_number ]);
        
        //      Failsafe
        if ( empty( $execution_steps_config[ $step_number ][ 'rendered_command' ]))
            return false;
        
        //      c) Optimization
        //      "Merge" consecutive steps happening in the same server (to make fewer SSH connections)
        //      -> Group rendered commands by server, but only for consecutive commands
        //      (can't break steps order, but numbering changes)
        if ( $id_server_previous_command == $step[ 'environment' ][ 'id_server' ])
            $grouped_rendered_commands[ $grouped_step_number ][] = $execution_steps_config[ $step_number ][ 'rendered_command' ];
        else
        {
            $grouped_rendered_commands[ $grouped_step_number ][] = $execution_steps_config[ $step_number ][ 'rendered_command' ];
            $grouped_step_number++;
        }
        
        $id_server_previous_command = $step[ 'environment' ][ 'id_server' ];
    }
    
    //      debug
    //krumo( 'prepare_batch_commands() - $execution_step_config :' );
    //krumo( $execution_step_config );
    
    //      d) Store into corresponding thread
    return db_update(
        'thread',
        array(
            'data' => safe_serialize( array(
                'current_step' => 0,
                'grouped_rendered_commands' => $grouped_rendered_commands,
                'execution_steps_config' => $execution_steps_config,
            )),
        ),
        array( 'id' => $batch[ 'id_thread' ])
    );
}



/**
 *  Get command configuration form (per batch step)
 *  @see engine/core/api_batch.inc
 */
function get_command_config_form_content( $command, $step_number = 0 )
{
    $form_content_html = "";
    
    if ( !empty( $command ))
    {
        //      Polymorphism : $command is either array or 'id_command'
        //      @see get_command()
        if ( is_numeric( $command ))
            $command = get_command( $id_command );
        
        //      For each custom token, determine & generate the appropriate form element
        if ( !empty( $command[ 'data' ][ 'tokens' ]))
            foreach( $command[ 'data' ][ 'tokens' ] as $token )
                switch( $token[ 'configuration' ])
                {
                    //      Selection of options
                    case 'options':
                    case 'multiple_options':
                        
                        $options_arr = options_to_arr( $token[ 'options' ]);
                        $select_options = array(
                            'select_attributes' => array(
                                'name' => 'custom-token-'. $token[ 'namespace' ] ."[$step_number]",
                                'value' => $token[ 'default_value' ],
                            ),
                        );
                        
                        if ( $token[ 'configuration' ] == 'multiple_options' )
                            $form_content_html .= multiple_select( "Options for <code>[". $token[ 'namespace' ] ."]</code>", $options_arr, $select_options );
                        else
                            $form_content_html .= select( "Options for <code>[". $token[ 'namespace' ] ."]</code>", $options_arr, $select_options );
                        
                        break;
                    
                    //      Text input
                    case 'raw':
                        
                        $form_content_html .= textarea( "Value for <code>[". $token[ 'namespace' ] ."]</code>", array( 'textarea_attributes' => array(
                            'name' => 'custom-token-'. $token[ 'namespace' ] ."[$step_number]",
                            'text' => $token[ 'default_value' ],
                        )));
                        
                        break;
                }
    }
    
    return $form_content_html;
}



/**
 *  Get command data
 */
function get_command( $id_command, $options = array())
{
    if ( !empty( $id_command ))
    {
        $command = db_fetch( "SELECT * FROM `command` WHERE `id_command` = ?", array( $id_command ));
        
        if ( !empty( $command[ 'data' ]))
            $command[ 'data' ] = safe_unserialize( $command[ 'data' ]);
        
        //      Command tags
        $command[ 'tags' ] = get_tags( 'command', $id_command );
        
        return $command;
    }
    return false;
}



/**
 *  Convert option string - e.g. "machine_value|Human value"
 *  into array of values - e.g. array( 'machine_value' => "Human value" )
 */
function options_to_arr( $options_str )
{
    $return = array();
    
    if ( !empty( $options_str ))
    {
        //      Replace line breaks by easier character to explode
        $options_str = strip_line_breaks( $options_str, '$$' );
        $lines = explode( '$$', $options_str );
        
        if ( !empty( $lines ))
            foreach( $lines as $l )
            {
                list( $key, $value ) = explode( '|', $l );
                $return[ trim( $key )] = trim( $value );
            }
    }
    
    return $return;
}


