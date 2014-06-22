<?php

/**
 *  @file
 *  Batch step
 *  @path rest/batch/123/step
 */

//      We just need to render batch
//      (self-contained contextual execution)
//      @see engine/core/api_batch_phases.inc
//      @see batch_processing_phase()
$content = batch_render( $arg1 );
//$content = batch_processing_phase( get_batch( $arg1 ));


