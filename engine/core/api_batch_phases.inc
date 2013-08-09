<?php

/**
 *  @file
 *  Batch API - Phases (states)
 *
 *  Contents :
 *      1. Setup (init)
 *      2. Review (standby)
 *      3. Execution (processing)
 *      4. Report (done)
 */


/**
 *  1. Setup (init)
 *
 *  @see engine/core/api_command.inc
 *  @see prepare_batch_commands()
 *  @see get_command_config_form_content()
 */
function batch_init_phase( $batch )
{
    $GLOBALS[ 'head_page_title' ] .= " - Setup (init)";
    
    //      This phase is in charge of :
    //          a) Locking the batch
    //          b) Presenting the commands configuration form (if applicable)
    //          c) "Merging" consecutive steps happening in the same server (to make fewer SSH connections)
    //          d) Preparing commands configuration form submission data for execution (store into 'thread' data)
    $html = '';
    $id_batch = $batch[ 'id_batch' ];

    //      Check if we submitted any data from commands configuration
    //      -> Prepare commands configuration form submission data for execution (store into 'thread' data),
    //      then go straight to recap (validation phase)
    //      @todo 2013/08/01 19:29:02 - handle special process : "all at once" (REST)
    if ( !empty( $_REQUEST[ 'batch_'. $id_batch .'_init_requested' ]))
    {
        $success_flag = prepare_batch_commands( $batch );

        if ( $success_flag )
        {
            //      Next phase
            $state_is_set = set_batch_state( $batch, 'standby' );
            if ( $state_is_set )
                return batch_render( $id_batch );
            else
                return alert( "Could not set batch <code>$id_batch</code> state to 'standby'", 'error' );
        }
        else
            return alert( "Error : can't prepare batch <code>$id_batch</code> for execution. That's all I know.", 'error' );
    }

    //      State display
    //$html .= "<p class='text-center muted'>Batch is in setup.</p>";

    //      From there, we "own" the lock
    set_batch_lock( $id_batch );

    //      Detect all commands contained in all steps,
    //      to see if there's any configuration to expose as a form
    $form_html = '';
    foreach( $batch[ 'steps' ] as $step_number => $step )
    {
        $step_form_content_html = get_command_config_form_content( $step[ 'command' ], $step_number );

        //      Indication : for which step / environment / server are we setting the configuration
        if ( !empty( $step_form_content_html ))
        {
            $form_html .= '<hr><p>Configuration options for step <code>'. $step_number .'</code> - Environment : '.
                '<strong>'.
                $step[ 'environment' ][ 'title' ] .
                '</strong>'.
                ' - Server : '.
                '<strong>'.
                $step[ 'environment' ][ 'server' ][ 'title' ] .
                '</strong>'.
                ' <code>'.
                $step[ 'environment' ][ 'server' ][ 'hostname' ] .
                '</code> '.
                '</p>'.
                view_source( $step[ 'command' ][ 'command' ], 'bash' ).
                $step_form_content_html;
        }
    }

    //      Render custom tokens configuration form (if applicable)
    if ( !empty( $form_html ))
    {
        //      Flag submission
        $form_html .= "<input type='hidden' name='batch_". $id_batch ."_init_requested' value='1'>";

        //      Render configuration form
        $html .= render_form( $form_html );
    }

    //      Easy case : no params for all commands in all steps
    //      -> Prepare commands configuration form submission data for execution (store into 'thread' data),
    //      then go straight to recap (validation phase)
    else
    {
        $success_flag = prepare_batch_commands( $batch );

        if ( $success_flag )
        {
            //      State display
            //$html .= "<p class='text-center muted'>(nothing to configure -> jump to 'standby' phase directly)</p>";

            //      Next phase
            $state_is_set = set_batch_state( $batch, 'standby' );
            if ( $state_is_set )
                return batch_render( $id_batch );
            else
                return alert( "Could not set batch <code>$id_batch</code> state to 'standby' while skipping configuration phase.", 'error' );
        }
        else
            $html .= alert( "Error : can't prepare batch <code>$id_batch</code> for execution. That's all I know.", 'error' );
    }

    return $html;
}



/**
 *  2. Review (standby) : Recap before launch
 */
function batch_standby_phase( $batch )
{
    $html = '';
    $id_batch = $batch[ 'id_batch' ];
    $GLOBALS[ 'head_page_title' ] .= " - Review (standby) : Recap before launch";

    //      Check if we submitted the "Go"
    if ( !empty( $_REQUEST[ 'everything-looks-good-do-it' ]))
    {
        //      Save as preset ?
        if ( !empty( $_REQUEST[ 'save-as-preset' ]))
            db_update( 'thread', array( 'preset' => 1 ), array( 'id' => $batch[ 'id_thread' ]));
        
        //      Next phase
        $state_is_set = set_batch_state( $batch, 'processing' );
        if ( $state_is_set )
            return batch_render( $id_batch );
        else
            return alert( "Could not set batch <code>$id_batch</code> state to 'processing'", 'error' );
    }

    //      Warning
    else if ( !empty( $_REQUEST[ 'poormans-submit-flag' ]))
        $html .= alert( "You need to tick the checkbox 'Everything looks good : do it' to begin execution phase.", 'warning' );

    //      State display
    //$html .= "<p class='text-center muted'>Batch is standing by.</p>";
    
    //      Load current step (with replaced tokens for commands)
    $batch_state_data = get_batch_state_data( $batch );
    
    //      Recap
    $commands_arr = array();
    foreach( $batch_state_data[ 'execution_steps_config' ] as $step )
        if ( !empty( $step[ 'rendered_command' ]))
            $commands_arr[] = '<hr><p>Environment : '.
                '<strong>'.
                $step[ 'environment' ][ 'title' ] .
                '</strong>'.
                ' - Server : '.
                '<strong>'.
                $step[ 'environment' ][ 'server' ][ 'title' ] .
                '</strong>'.
                ' <code>'.
                $step[ 'environment' ][ 'server' ][ 'hostname' ] .
                '</code> '.
                '</p>'.
                view_source( $step[ 'rendered_command' ], 'bash' ) .
                '';

    //      Display
    $html .= "<p>Please review the list of operations that will be executed :</p>";
    $html .= "<ol><li>". join( '</li><li>', $commands_arr ) ."</li></ol>";
    $html .= "<hr>";
    $html .= "<h2>Ready ?</h2>";

    //      Execution form
    $execution_form_html = "";
    $execution_form_html .= '<div class="control-group">';
    $execution_form_html .= '<label class="control-label">&nbsp;</label>';
    $execution_form_html .= '<div class="controls">';
    $execution_form_html .= input_checkbox( "Everything looks good : do it" );
    $execution_form_html .= input_checkbox( "Save as preset" );
    $execution_form_html .= '</div>';
    $execution_form_html .= '</div>';
    $html .= render_form( $execution_form_html );

    return $html;
}



/**
 *  3. Execution (processing)
 *
 *  Dual context :
 *      a) Render "wrapping" ajax interface
 *      b) Process ajax steps in REST calls
 *
 *  Then "Move" to next phase when it's over
 */
function batch_processing_phase( $batch )
{
    $html = '';
    $id_batch = $batch[ 'id_batch' ];
    $GLOBALS[ 'head_page_title' ] .= " - Execution (processing)";
    
    //      debug
    //krumo( $batch );
    
    //      Load current step (with replaced tokens for commands)
    $batch_state_data = get_batch_state_data( $batch );
    
    //      debug
    //krumo( $batch_state_data );
    
    //      State display
    //$html .= "<p class='text-center muted'>Batch is processing.</p>";
    
    //      Setup for Ajax
    $iteration_delay = 1000;
    $ajax_batch_processing_step_url = base_path() ."rest/batch/$id_batch/step";
    $last_iteration = count( $batch_state_data[ 'grouped_rendered_commands' ]);
    
    //      Context separation :
    //      a) Render "wrapping" ajax interface
    $arg2 = arg( 2 );
    if ( $arg2 != 'step' )
    {
        //      First, check if we're done (even outside REST step-by-step calls, just in case)
        if ( $batch_state_data[ 'current_step' ] >= $last_iteration )
        {
            //      Next phase
            $state_is_set = set_batch_state( $batch, 'processed' );
            if ( $state_is_set )
                return batch_render( $id_batch );
            else
                return alert( "Could not set batch <code>$id_batch</code> state to 'processed'", 'error' );
        }
        
        //      <Html-Ajax-Interface>
        $html .= <<<EOD
<h2>Ajax processing</h2>
<hr>
<div class="row-fluid">
    <div id="batch-execution-status" class="span4">
        <div style="text-align:center">
            <a class="btn btn-primary btn-large" id="btn-execute" href="javascript:void(0)">
                <i class="icon-bolt icon-large"></i>&nbsp;Launch Ajax Execution
            </a>
        </div>
    </div>
    <div id="batch-execution-zone" class="span8">
        <p style="text-align:center"> (batch Ajax execution standing by) </p>
    </div>
</div>
<p style="padding-bottom:100px">&nbsp;</p>
EOD;
        //      </Html-Ajax-Interface>
        
        //      <Javascript-Ajax-Interface>
        $GLOBALS[ 'footer_extra_js' ] .= '<script src="'. base_path() .'engine/core/js/api_batch.js"></script>';
        $GLOBALS[ 'footer_extra_js' ] .= <<<EOD
<script type="text/javascript">

$( "#btn-execute" ).click( function()
{
    if ( $(this).hasClass( 'disabled' ))
        return false;
    
    execute_batch({
        step: 0,
        batch_url: '$ajax_batch_processing_step_url',
        total: $last_iteration,
        id_feedback: 'batch-execution-status',
        id_zone: 'batch-execution-zone',
        delay: $iteration_delay
    });
    
    $(this).addClass( 'disabled' );
});

</script>
EOD;
        //      </Javascript-Ajax-Interface>
    }
    
    //      Context REST
    //      b) Process steps
    else
    {
        //      Failsafe
        if ( !empty( $batch_state_data[ 'execution_steps_config' ][ $batch_state_data[ 'current_step' ]]) &&
            !empty( $batch_state_data[ 'grouped_rendered_commands' ][ $batch_state_data[ 'current_step' ]]))
        {
            //      We are inside a REST request : flush previous HTML
            $html = '';
            
            //      This step data
            $current_step_data = $batch_state_data[ 'execution_steps_config' ][ $batch_state_data[ 'current_step' ]];
            $current_step_rendered_commands = $batch_state_data[ 'grouped_rendered_commands' ][ $batch_state_data[ 'current_step' ]];
            
            //      Progress bar
            //      Note : current step + 1 because it will be displayed when it's done, and to handle last step 100% properly
            $percent = (( $batch_state_data[ 'current_step' ] + 1 ) / $last_iteration ) * 100;
            $html .= progress_bar( $percent );
            //$html .= "<p class='text-center' style='margin-top:0'>$percent&nbsp;%</p>";
            $html .= "<p>Here's a brief preview of Shell \"stdout\" logs for this step : </p>";
            
            //      Connection
            $ssh = ssh( $current_step_data[ 'environment' ][ 'id_server' ]);

            //      In each step - or "grouped" step
            //      @see prepare_batch_commands()
            //      We might find several commands to execute at once (re-using ssh connection)
            foreach( $current_step_rendered_commands as $cmd )
            {
                //      Execute command
                //      Handle debug
                if ( !empty( $GLOBALS[ 'debug' ][ 'prevent_ssh_exec' ]))
                    $std_out = "*** Debug ". datestamp_format() ." : preventing SSH commands execution ***";
                else
                    $std_out = $ssh->exec( $cmd );
                
                //      Preview
                $html .= view_source( $std_out, 'bash' );
                
                //      Log the results
                $msg = "Batch $id_batch - Thread ". $batch[ 'id_thread' ] ." - Step ". $batch_state_data[ 'current_step' ];
                $inserted_id = linked_dblog( $batch[ 'id_thread' ], 'thread', $msg, array( 'data' => array(
                    'cmd' => $cmd,
                    'std_out' => $std_out,
                    'id_environment' => $current_step_data[ 'environment' ][ 'id_environment' ],
                    'id_batch' => $id_batch,
                )));
                
                //      Failsafe
                if ( !$inserted_id )
                    return alert( "Error : impossible to log the outcome of $msg", 'error' );
                
                //      @evol 2013/08/04 17:12:51 error detection : pause batch ? -> resume ?
                //if ( strpos( strtolower( $std_out ), 'error' ))
                //  $GLOBALS[ 'footer_extra_js' ] .= '<script> window.batch_killswitch = true; </script>';
            }
            
            //      Is it over yet ?
            if ( $batch_state_data[ 'current_step' ] >= ( $last_iteration - 1 ))
            {
                //      Next phase
                $state_is_set = set_batch_state( $batch, 'processed' );
                
                //      We are inside a REST callback
                if ( $state_is_set )
                    $GLOBALS[ 'footer_extra_js' ] .= "<script> setTimeout( function() { location.reload(); }, $iteration_delay ); </script>";
                else
                    return alert( "Could not set batch <code>$id_batch</code> state to 'processed' when reached last processing step.", 'error' );
            }
            
            //      Update data for next step
            else
            {
                $batch_state_data[ 'current_step' ]++;
                db_update( 'thread', array( 'data' => safe_serialize( $batch_state_data )), array( 'id' => $batch[ 'id_thread' ]));
            }
        }
        
        //      Assume it's over (no step data = no step left to execute)
        /*
        else
        {
            //if ( $batch_state_data[ 'current_step' ] >= $last_iteration )
            //{
                //      Next phase
                $state_is_set = set_batch_state( $id_batch, 'processed' );
                
                //      We are inside a REST callback
                if ( $state_is_set )
                    $GLOBALS[ 'footer_extra_js' ] .= '<script> setTimeout( function() { location.reload(); }, o.delay ); </script>';
                else
                    return alert( "Error : impossible to set batch state for <code>$id_batch</code>", 'error' );
            //}
        }
        */
    }

    return $html;
}



/**
 *  4. Report (done)
 */
function batch_done_phase( $batch )
{
    $html = '';
    $id_batch = $batch[ 'id_batch' ];
    $GLOBALS[ 'head_page_title' ] .= " - Report (done)";
    
    //      State display
    //$html .= "<p>Batch is done.</p>";
    
    //      debug
    //$state_is_set = 1;
    //krumo( 'batch_done_phase()' );
    //krumo( $batch );
    
    //      update 2013/08/05 01:44:19 - can't set 'over' state here
    //      But we need an extra state to manage last display (report)
    //$state_is_set = set_batch_state( $id_batch, 'over' );
    
    //      Failsafe
    //if ( $state_is_set )
    //{
        $dblog_entries = get_batch_log( $batch[ 'id_thread' ]);
        $preprocessed_log_entries_arr = array();
        
        //      debug
        //krumo( $dblog_entries );
        
        foreach( $dblog_entries as $step => $log )
        {
            $env_data = db_fetch( "SELECT * FROM `environment` WHERE `id_environment` = ?", array( $log[ 'id_environment' ]));
            $server_data = db_fetch( "SELECT * FROM `server` WHERE `id_server` = ?", array( $env_data[ 'id_server' ]));
            
            $preprocessed_log_entries_arr[] = '<hr><p>Context :<br>Environment : '.
                '<strong>'.
                $env_data[ 'title' ] .
                '</strong>'.
                ' - Server : '.
                '<strong>'.
                $server_data[ 'title' ] .
                '</strong>'.
                ' <code>'.
                $server_data[ 'hostname' ] .
                '</code> '.
                '<br>
                Raw command <span class="muted">(as it was actually executed)</span> :'.
                '</p>'.
                view_source( $log[ 'cmd' ], 'bash' ) .
                '<p>'.
                "<strong>'Std out'</strong> feedback :".
                '</p>'.
                view_source( $log[ 'std_out' ], 'bash' ) .
                '';
        }
        
        $html .= "<p>Process is over. Here's the report</p>";
        $html .= "<ol><li>". join( '</li><li>', $preprocessed_log_entries_arr ) ."</li></ol>";
    //}
    //else
    //    return alert( "Error : impossible to set batch state for <code>$id_batch</code>" );
    
    //      Clear optionally cached data
    if ( cache_get( "dump_filename_thread_". $batch[ 'id_thread' ]))
        cache_clear( "dump_filename_thread_". $batch[ 'id_thread' ]);
    
    return $html;
}


