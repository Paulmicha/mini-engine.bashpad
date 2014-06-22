<?php

/**
 *  @file
 *  Batch - Presets rendering
 *  
 *  Steps :
 *      1. Check if batch is locked by someone else
 *      2. Replace datestamps
 *      3. Copy preset into new thread & set it as the thread for the batch
 *      4. Carry on the normal batch render process from 'standby' state
 *  
 *  @path /batch/presets/123
 */

//      Init
$head_page_title = "<i class='icon-road'></i> Execute Preset <code>$arg2</code>";

//      Load preset thread data
$preset = db_fetch( "SELECT * FROM `thread` WHERE `id_thread` = ?", array( $arg2 ));
if ( empty( $preset ))
    $content .= alert( "No thread data was found with preset id <code>$arg2</code>", 'warning' );
else
{
    //      Read serialized thread data
    if ( !empty( $preset[ 'data' ]))
        $preset[ 'data' ] = safe_unserialize( $preset[ 'data' ]);
    
    //      Get batch data
    $id_batch = $preset[ 'id_batch' ];
    $batch = get_batch( $id_batch );
    
    //      debug
    //krumo( $preset );
    //krumo( $batch );
    
    
    
    //-------------------------------------------------------------------------------------
    //      1. Check if batch is locked by someone else
    
    
    if ( $_REQUEST[ 'steal' ] == 'lock' )
    {
        //      Release the lock
        release_batch_lock( $id_batch );
        
        //      Reset batch state
        $state = 'init';
        $state_is_set = set_batch_state( $batch, $state );
        
        //      Failsafe
        if ( !$state_is_set )
            $content .= alert( "Could not set batch <code>$id_batch</code> in state 'init' while trying to steal the lock.", 'error' );
    }
    else if ( batch_is_locked_by_someone_else( $id_batch ))
    {
        $launched_by = cache_get( "batch_lock_$id_batch" );
        if ( empty( $launched_by ))
            $launched_by = "someone else.";
        else
            $launched_by = " <code> $launched_by </code> ";
        
        $content .= alert( "This batch '". $batch[ 'title' ] ."' <code>($id_batch)</code> is locked by $launched_by <i class='icon-lock icon-large' style='margin-left:2em'></i>", 'warning' );
        $content .= "<p class='text-center'><i class='icon-time icon-large'></i> Current state : <code>". get_batch_state( $batch ) ."</code>. </p>";
        
        //      Force release lock
        $content .= "<hr>";
        $content .= "<h2>";
        $content .= "Stuck ?";
        $content .= " <i class='icon-long-arrow-right'></i> ";
        $content .= l(
            "<i class='icon-unlock icon-large'></i> Break lock",
            get_current_url( array( 'no_vars' => 1 )) ."?steal=lock",
            array( 'attributes' => array( 'class' => 'btn btn-large btn-inverse' ))
        );
        $content .= "</h2>";
    }
    
    
    
    //-------------------------------------------------------------------------------------
    //      2. Replace datestamps
    
    
    //      @todo 2013/08/12 23:43:44
    
    
    
    //-------------------------------------------------------------------------------------
    //      3. Copy preset into new thread & set it as the thread for the batch
    
    
    //      We might have begun this batch before and not finished it,
    //      which is a case undetected by the "lock" -> deal with it first
    $thread_over_check = db_popfetch( "SELECT `state` FROM `thread` WHERE `id_thread` = ?", array( $batch[ 'id_thread' ]));
    if ( $thread_over_check != 'over' )
    [
        $content .= alert( "You must have started this batch without cancelling it before trying to launch this preset.", 'warning' );
        $content .= "<p>You need to ". l( "<i class='icon-remove'></i>&nbsp;cancel this batch", "batch/$id_batch/render?cancel-batch=$id_batch", array( 'attributes' => array( 'class' => "btn btn-warning" ))) ." first.</p>";
    ]
    else
    {
        $id_thread = db_insert( 'thread', array(
            'id_batch' => $id_batch,
            'datestamp' => datestamp(),
            
            //      Copy the preset data
            'data' => safe_serialize( $preset[ 'data' ]);
        ));
        
        //      Failsafe : Batch must have 'id_thread' by then
        if ( !empty( $id_thread ))
            db_update( 'batch', array( 'id_thread' => $id_thread ), array( 'id' => $id_batch ));
        else
            $content .= alert( "Could not attach a thread to this batch. That's all I know.", 'error' );
        
        //      Update batch data
        $batch[ 'id_thread' ] = $id_thread;
        
        
        
        //-------------------------------------------------------------------------------------
        //      4. Carry on the normal batch render process from 'standby' state
        
        
        $state_is_set = set_batch_state( $batch, 'standby' );
        if ( $state_is_set )
        {
            header( "Location: ". url( "batch/$id_batch/render", array( 'absolute' => true )));
            die();
        }
        else
            $content .= alert( "Could not set batch <code>$id_batch</code> state to 'standby' while finishing preset <code>$arg2</code> preparation.", 'error' );
    }
}


