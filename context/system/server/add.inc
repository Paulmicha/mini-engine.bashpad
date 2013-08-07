<?php

/**
 *  @file
 *  Server - add
 *  
 *  @path /sytem/server/add
 */

//      Init
$head_page_title = "<i class='icon-plus'></i> Add new server";

//      Render form
$form_html_content = _server_form_content();
$content .= render_form( $form_html_content, '_server_form_submit', array(
    'validation_handler' => '_server_form_validate',
    
    //      This allows us to re-use the same validation function for both creation and edition forms
    'submit_handler_params' => array( 'is_new' => 1 ),
));


