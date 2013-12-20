<?php

/**
 *  @file
 *  Commands - add
 *  @path /sytem/command/add
 */

//      Init
$head_page_title = "<i class='icon-plus'></i> Add new command";

//      Render form
$form_html_content = _command_form_content();
$content .= render_form( $form_html_content, '_command_form_submit', array(
    'validation_handler' => '_command_form_validate',
    'submit_handler_params' => array( 'is_new' => 1 ),
));


