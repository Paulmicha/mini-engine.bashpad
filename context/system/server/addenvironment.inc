<?php

/**
 *  @file
 *  Environment - add environment
 *  @path /sytem/environment/123/add-environment
 */

//      Init
$head_page_title = "Add new environment for server $server_title";

//      Render form
$form_html_content = _environment_form_content();
$content .= render_form( $form_html_content, '_environment_form_submit', array(
    'validation_handler' => '_environment_form_validate',
    'submit_handler_params' => array( 'is_new' => 1 ),
));


