<?php

/**
 *  @file
 *  batch - edit
 *  @path /sytem/batch/1/edit
 */

//      Init
$head_page_title = "Edit batch $batch_title";

//      Loading data
$batch_data = db_fetch( "SELECT * FROM `batch` WHERE `id_batch`=$arg1" );

//      debug
//krumo( $batch_data );

if ( empty( $batch_data ))
    $content .= alert( "batch not found.", 'error' );
else
{
    $head_page_title = "Manage ". $batch_data[ 'title' ];
    
    //      Multiselect : Tag(s) mapping preparation
    $tags_batch = get_tags( 'batch', $arg1 );
    $tags_batch_arr = array();
    foreach( $tags_batch as $id_tag )
        $tags_batch_arr[] = "id-$id_tag";
    
    //      Multigroup : Batch steps mapping preparation
    $batch_steps_mapping = array();
    $batch_steps_arr = db_fetchall( "SELECT * FROM `batch_step` WHERE `id_batch`=$arg1 ORDER BY `step` ASC" );

    //      debug
    //krumo( $batch_steps_arr );
    
    if ( !empty( $batch_steps_arr ))
        foreach( $batch_steps_arr as $i => $step )
        {
            $batch_steps_mapping[ 'multifield-batch-steps-step-number' ][ 'multigroup-'. $i ] = $step[ 'step' ];
            $batch_steps_mapping[ 'multifield-batch-steps-step-title' ][ 'multigroup-'. $i ] = $step[ 'title' ];
            $batch_steps_mapping[ 'multifield-batch-steps-environment' ][ 'multigroup-'. $i ] = $step[ 'id_environment' ];
            $batch_steps_mapping[ 'multifield-batch-steps-command' ][ 'multigroup-'. $i ] = db_popfetch( "SELECT `title` FROM `command` WHERE `id_command` = ?", array( $step[ 'id_command' ])) ." [". $step[ 'id_command' ] ."]";
        }
    
    //      Mapping
    $form_values = array(
        'id_batch' => $batch_data[ 'id_batch' ],
        'title' => $batch_data[ 'title' ],
        'description' => $batch_data[ 'description' ],
        'tag-s-' => $tags_batch_arr,
        'multigroups' => array(
            'batch_steps' => $batch_steps_mapping,
        ),
    );
    
    //      Render form
    $content .= render_form(
        form_set_values( _batch_form_content(), $form_values ),
        '_batch_form_submit',
        array(
            'validation_handler' => '_batch_form_validate',
            'submit_handler_params' => array( 'id_batch' => $batch_data[ 'id_batch' ]),
        )
    );
}



//------------------------------------------------------------------------------------------------------------
//      Autocompletion


$max = 999;
$commands_results = db_fetchall( "SELECT * FROM `command` ORDER BY `title` ASC LIMIT 0,$max" );

$inline_str = '';
$inline_arr = array();

if ( !empty( $commands_results ))
    foreach( $commands_results as $r )
        $inline_arr[] = addcslashes( $r[ 'title' ] .' ['. $r[ 'id_command' ] .']', '"' );

$inline_str = '"'. join( '","', $inline_arr ) .'"';


//      Javascript : Autocompletion Commands
$footer_extra_js .= <<<EOD
<script type="text/javascript">
(function($)
{
    //      Init autocomplete
    var cmd_arr = [ $inline_str ];
    $( 'input.autocompletion-command' ).typeahead({ source:cmd_arr });
    
    //      Re-init when multigrtoup add btn is clicked
    $( '.repeatable-field-add' ).click( function() {
        $( 'input.autocompletion-command' ).typeahead({ source:cmd_arr });
    });
    
})(jQuery);
</script>
EOD;


