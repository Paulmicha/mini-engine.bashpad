<?php

/**
 *  @file
 *  batch - add
 *  @path /sytem/batch/add
 */

//      Init
$head_page_title = "<i class='icon-plus'></i> Create a new batch";

//      Description
$content .= "<p>This is where we setup a new set of operations to run in a batch process.</p>";

//      Render form
$content .= render_form( _batch_form_content(), '_batch_form_submit', array(
    'validation_handler' => '_batch_form_validate',
    'submit_handler_params' => array( 'is_new' => 1 ),
    'form_attributes' => array( 'attributes' => array( 'class' => 'batch-add-form form-horizontal' )),
));

//      Let's see if this holds for now
//      @evol 2013/07/29 17:36:44 - ajaxify autocompletion
$max = 999;
$commands_results = db_fetchall( "SELECT * FROM `command` ORDER BY `title` ASC LIMIT 0,$max" );

$inline_str = '';
$inline_arr = array();

if ( !empty( $commands_results ))
    foreach( $commands_results as $r )
        $inline_arr[] = addcslashes( $r[ 'title' ] .' ['. $r[ 'id_command' ] .']', '"' );

$inline_str = '"'. join( '","', $inline_arr ) .'"';

//      Javascript : Autocompletion init var
$footer_extra_js .= <<<EOD
<script type="text/javascript">
window.autocomplete_cmd_arr = [ $inline_str ];
</script>
EOD;

//      Javascript : Autocompletion Commands + Hidden chosen bugfix
$footer_extra_js .= '<script src="'. base_path() .'engine/core/js/batch_crud.js"></script>';


