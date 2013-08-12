<?php

/**
 *  @file
 *  Batch API
 */


/**
 *  Render batch (auto steps detection)
 *  
 *  Phases (states) :
 *      1. Setup (init)
 *      2. Review (standby)
 *      3. Execution (processing)
 *      4. Report (done)
 *  
 *  @see engine/core/api_batch_phases.inc
 */
function batch_render( $id_batch, $options = array())
{
    //      Overridable defaults
    //      @todo 2013/08/01 13:14:48 - types of batch operations : commands OR custom callbacks + custom settings
    //      e.g. to handle CSV operations, etc.
    //$options += array(
    //    'state' => 'init',
    //);
    //extract( $options );
    
    $html = '';
    $batch = get_batch( $id_batch );
    
    //      Failsafe : Batch must have steps
    if ( empty( $batch[ 'steps' ]))
        return alert( "No execution steps were found in this batch - there's nothing to do here, " ) .
            "<p>". l( "except editing this batch to add steps", "batch/$id_batch/edit" ) ."</p>";
    
    
    //------------------------------------------------------------
    //      Lock
    
    //      Failsafe : Batch is locked by someone else
    //      Did we request to steal the lock ?
    if ( $_REQUEST[ 'steal' ] == 'lock' )
    {
        //      Release the lock
        release_batch_lock( $id_batch );
        
        //      Reset batch state
        $state = 'init';
        //$state_is_set = set_batch_state( $id_batch, $state );
        $state_is_set = set_batch_state( $batch, $state );
        
        //      Failsafe
        if ( !$state_is_set )
            return alert( "Could not set batch <code>$id_batch</code> in state 'init' while trying to steal the lock.", 'error' );
    }
    else if ( batch_is_locked_by_someone_else( $id_batch, $options ))
    {
        $launched_by = cache_get( "batch_lock_$id_batch" );
        if ( empty( $launched_by ))
            $launched_by = "someone else.";
        else
            $launched_by = " <code> $launched_by </code> ";
        
        $html .= alert( "This batch '". $batch[ 'title' ] ."' <code>($id_batch)</code> is locked by $launched_by <i class='icon-lock icon-large' style='margin-left:2em'></i>", 'warning' );
        $html .= "<p class='text-center'><i class='icon-time icon-large'></i> Current state : <code>". get_batch_state( $batch ) ."</code>. </p>";
        
        //      Force release lock
        $html .= "<hr>";
        $html .= "<h2>";
        $html .= "Stuck ?";
        $html .= " <i class='icon-long-arrow-right'></i> ";
        $html .= l(
            "<i class='icon-unlock icon-large'></i> Break lock",
            get_current_url( array( 'no_vars' => 1 )) ."?steal=lock",
            array( 'attributes' => array( 'class' => 'btn btn-large btn-inverse' ))
        );
        $html .= "</h2>";
        
        return $html;
    }
    
    
    //------------------------------------------------------------
    //      Thread
    
    //      Side note : the word "thread" is used for its meaning of "tying pieces together"
    //      @see http://www.wordreference.com/enfr/thread
    $thread_over_check = '';
    if ( !empty( $batch[ 'id_thread' ]))
        $thread_over_check = db_popfetch( "SELECT `state` FROM `thread` WHERE `id_thread` = ?", array( $batch[ 'id_thread' ]));
    
    //      We need an "n - 1" relation between 'thread' and 'batch',
    //      so that we can archive != presets (in serialized data on thread) per batch,
    //      but we need to maintain a single 'active' thread per batch
    //      (~ singleton : can't have multiple 'instances' of the same batch running simultaneously)
    if ( empty( $batch[ 'id_thread' ]) || $thread_over_check == 'over' )
    {
        $id_thread = db_insert( 'thread', array( 'id_batch' => $id_batch, 'datestamp' => datestamp()));
        
        //      Failsafe : Batch must have 'id_thread' by then
        if ( !empty( $id_thread ))
            db_update( 'batch', array( 'id_thread' => $id_thread ), array( 'id' => $id_batch ));
        else
            return alert( "Could not attach a thread to this batch. That's all I know.", 'error' );
        
        //      Update batch data
        $batch[ 'id_thread' ] = $id_thread;
    }
    
    
    //------------------------------------------------------------
    //      State
    
    $state = get_batch_state( $batch );
    if ( empty( $state ) || $state == 'over' )
    {
        $state = 'init';
        $state_is_set = set_batch_state( $batch, $state );
        
        //      Failsafe
        if ( !$state_is_set )
            return alert( "Could not set batch <code>$id_batch</code> back to state 'init' to start it over.", 'error' );
    }
    
    //      Contextual rendering
    //      @see engine/core/api_batch_phases.inc
    $phase_state_handler = 'batch_'. $state .'_phase';
    if ( function_exists( $phase_state_handler ))
        $html .= $phase_state_handler( $batch );
    
    //      Special state : the LAST step must only be set AFTER the above "dispatch" of state-dependent processes
    if ( $state == 'done' )
    {
        //      Final, extra state
        $state_is_set = set_batch_state( $batch, 'over' );
        if ( $state_is_set )
        {
            //      Release Lock
            release_batch_lock( $id_batch );
        }
        else
            return alert( "Could not set batch <code>$id_batch</code> in state 'over'", 'error' );
    }
    
    //      Need another state just before, because of the last 'processing' display (100%) + reload
    else if ( $state == 'processed' )
    {
        $state_is_set = set_batch_state( $batch, 'done' );
        if ( !$state_is_set )
            return alert( "Could not set batch <code>$id_batch</code> in state 'done'", 'error' );
    }
    
    return $html;
}



/**
 *  Batch get
 */
function get_batch( $id_batch, $options = array())
{
    //      Overridable defaults
    //$options += array(
    //);
    //extract( $options );
    
    //      Fetch DB data
    $batch_data = db_fetch( "SELECT * FROM `batch` WHERE `id_batch` = ?", array( $id_batch ));
    $batch_steps_arr = db_fetchall( "SELECT * FROM `batch_step` WHERE `id_batch` = ? ORDER BY `step` ASC", array( $id_batch ));
    
    //      Batch tags
    $batch_data[ 'tags' ] = get_tags( 'batch', $id_batch );
    
    //      Load steps commands + commands data
    if ( !empty( $batch_steps_arr ))
        foreach( $batch_steps_arr as $i => $step )
        {
            //      Load command data
            //      @see engine/core/api_command.inc
            if ( !empty( $step[ 'id_command' ]))
                $batch_steps_arr[ $i ][ 'command' ] = get_command( $step[ 'id_command' ]);
            
            //      Load environment data
            if ( !empty( $step[ 'id_environment' ]))
            {
                $batch_steps_arr[ $i ][ 'environment' ] = db_fetch( "SELECT * FROM `environment` WHERE `id_environment` = ?", array( $step[ 'id_environment' ]));
                
                //      Server
                $batch_steps_arr[ $i ][ 'environment' ][ 'server' ] = db_fetch( "SELECT * FROM `server` WHERE `id_server` = ?", array( $batch_steps_arr[ $i ][ 'environment' ][ 'id_server' ]));
                if ( !empty( $batch_steps_arr[ $i ][ 'environment' ][ 'server' ][ 'data' ]))
                    $batch_steps_arr[ $i ][ 'environment' ][ 'server' ][ 'data' ] = safe_unserialize( $batch_steps_arr[ $i ][ 'environment' ][ 'server' ][ 'data' ]);
                
                //      Folders
                $folders = db_fetchall( "SELECT * FROM `folder` WHERE `id_environment` = ?", array( $step[ 'id_environment' ]));
                if ( !empty( $folders ))
                {
                    //      Folders tags
                    foreach( $folders as $j => $folder )
                        $folders[ $j ][ 'tags' ] = get_tags( 'folder', $folder[ 'id_folder' ]);
                    
                    $batch_steps_arr[ $i ][ 'environment' ][ 'folders' ] = $folders;
                }
                
                //      Credentials
                if ( !empty( $batch_steps_arr[ $i ][ 'environment' ][ 'data' ]))
                    $batch_steps_arr[ $i ][ 'environment' ][ 'data' ] = safe_unserialize( $batch_steps_arr[ $i ][ 'environment' ][ 'data' ]);
                
                //      Environment tags
                $batch_steps_arr[ $i ][ 'environment' ][ 'tags' ] = get_tags( 'environment', $step[ 'id_environment' ]);
            }
        }
    
    return array( 'steps' => $batch_steps_arr ) + $batch_data;
}



/**
 *  Get batch log data (for 'done' phase)
 *  
 *  @see engine/core/api_batch_phases.inc
 *  @see batch_done_phase()
 */
function get_batch_log( $id_thread )
{
    $logs = array();
    $dblog_entries = array();
    
    $log_ids = db_fetchall( "SELECT has_dblog.id_dblog
        FROM has_dblog
        WHERE has_dblog.id = ?",
        array( $id_thread ));
    
    if ( !empty( $log_ids ))
        foreach( $log_ids as $log_id )
            $logs[] = db_fetch( "SELECT data FROM dblog WHERE id_dblog = ?", array( $log_id[ 'id_dblog' ]));
    
    if ( !empty( $logs ))
        foreach( $logs as $log )
            $dblog_entries[] = safe_unserialize( $log[ 'data' ]);
    
    return $dblog_entries;
}



/**
 *  Get state data
 */
function get_batch_state_data( $batch )
{
    //      Polymorphism : param $batch either Integer - id_batch,
    //      OR array - result of get_batch()
    if ( !empty( $batch ))
    {
        if ( is_numeric( $batch ))
            $batch = get_batch( $batch );
        
        if ( !empty( $batch[ 'id_thread' ]))
            return safe_unserialize( db_popfetch( "SELECT data FROM thread WHERE id_thread = ?", array( $batch[ 'id_thread' ])));
    }
    
    return false;
}



/**
 *  Get state
 *  @return String 'init' || 'standby' || 'processing' || 'done' || 'over'
 */
function get_batch_state( $batch )
{
    //      Polymorphism : param $batch either Integer - id_batch,
    //      OR array - result of get_batch()
    if ( !empty( $batch ))
    {
        if ( is_numeric( $batch ))
            $batch = get_batch( $batch );
        
        if ( !empty( $batch[ 'id_thread' ]))
            return db_popfetch( "SELECT state FROM thread WHERE id_thread = ?", array( $batch[ 'id_thread' ]));
    }
    
    return false;
}



/**
 *  Set state
 *  $state can be 'init' || 'standby' || 'processing' || 'done' || 'over'
 */
function set_batch_state( $batch, $state )
{
    if ( !empty( $batch[ 'id_thread' ]))
        return db_update( 'thread', array( 'state' => $state ), array( 'id' => $batch[ 'id_thread' ]));
    
    return false;
}



/**
 *  Check if this batch is locked (by someone else)
 */
function batch_is_locked_by_someone_else( $id_batch, $options = array())
{
    //      Ownership by IP
    $owner_ip = cache_get( "batch_lock_$id_batch" );
    if ( !empty( $owner_ip ) && $owner_ip == ip_address())
        return false;
    
    //      Otherwise check batch state : a batch is only considered 'free' when state is either 'over' or empty
    $state = get_batch_state( $id_batch );
    if ( empty( $state ) || $state == 'over' )
        return false;
    
    return true;
}



/**
 *  Set Lock
 */
function set_batch_lock( $id_batch, $options = array())
{
    //      Overridable defaults
    $options += array(
        'ip' => ip_address(),
        'unique_batch_id' => unique_batch_id( $id_batch ),
    );
    extract( $options );
    
    //      Set in cache to be able to break it if needed
    cache_set( "batch_lock_$id_batch", $ip );
}



/**
 *  Release Lock
 */
function release_batch_lock( $id_batch ) {
    cache_clear( "batch_lock_$id_batch" );
}



/**
 *  Batch unique ID (per IP)
 */
function unique_batch_id( $id_batch ) {
    return "datestamp:". datestamp() ."-id_batch:$id_batch-ip:". ip_address();
}


