<?php

/**
 *  @file
 *  Test : form - Multigroup (Repeatable fields)
 *  
 *  @path /system/test/form-multigroup
 */

//      Init
$form_content_html = '';
$head_page_title = "Test form - Multigroup (Repeatable fields)";
$form_content_html .= grid( array(
    array(  "<h2>Result</h2>", "<h2>Snippet</h2>" ),
));

//      Repeatable fields
$code = <<<'EOT'
multigroup( "Multigroup", array(
    array(
        'func' => 'input_text',
        'args' => array( "Repeatable text input" ),
    ),
    array(
        'func' => 'select',
        'args' => array( "Repeatable select", array( 'option 1', 'option 2', 'option 3' )),
    ),
    array(
        'func' => 'input_radios',
        'args' => array( 'Repeatable Radios Group', array( 'option 1', 'option 2', 'option 3' )),
    ),
    array(
        'func' => 'input_checkboxes',
        'args' => array( 'Repeatable Checkboxes Group', array( 'option 1', 'option 2', 'option 3' )),
    ),
));
EOT;

$repeatable_fields_html = multigroup( "Multigroup", array(
    array(
        'func' => 'input_text',
        'args' => array( "Repeatable text input" ),
    ),
    array(
        'func' => 'textarea',
        'args' => array( "Repeatable textarea" ),
    ),
    array(
        'func' => 'select',
        'args' => array( "Repeatable select", array( 'option 1', 'option 2', 'option 3' )),
    ),
    array(
        'func' => 'input_radios',
        'args' => array( 'Repeatable Radios Group', array( 'option 1', 'option 2', 'option 3' )),
    ),
    array(
        'func' => 'input_checkboxes',
        'args' => array( 'Repeatable Checkboxes Group', array( 'option 1', 'option 2', 'option 3' )),
    ),
));

$form_content_html .= grid( array(
    array( $repeatable_fields_html, view_source( $code )),
));


//      hack 2013/07/24 13:54:28 - since default classes 'span5' to make forms look good in single-column layouts,
//      need to reset those in this mutli-columns layout
$form_content_html = str_replace( 'span5', '', $form_content_html );


//      Render form
$content .= $form_content_html;


