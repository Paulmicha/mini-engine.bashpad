<?php

/**
 *  @file
 *  Test : form theming
 *  @path /system/test/form-theming
 */

//      Init
$head_page_title = "Test Form theming";
$form_content_html = '';

$form_content_html .= grid( array(
    array(  "<h2>Result</h2>", "<h2>Snippet</h2>" ),
));


//      Input text
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_text( 'Input text' );
EOT;
$form_content_html .= grid( array(
    array( input_text( 'Input text' ), view_source( $code )),
));


//      Input text attributes demo
$form_content_html .= '<hr />';
$code = <<<'EOT'
//      More details in source file comments
//      @see /engine/core/api_form.inc
input_text( 'Input text attributes demo', array(
    'input_attributes' => array(
        'help-inline' => "Help inline",
        'help-block' => "Help block",
        'placeholder' => "Overridden placeholder",
        'style' => "color:limegreen",
        'data-custom-attribute' => "custom value",
    ),
));
EOT;
$form_content_html .= grid( array(
    array( input_text( 'Input text attributes demo', array( 'input_attributes' => array(
            'help-inline' => "Help inline",
            'help-block' => "Help block",
            'placeholder' => "Overridden placeholder",
            'style' => "color:limegreen",
            'data-custom-attribute' => "custom value",
        ))), view_source( $code )),
));


//      Textarea
$form_content_html .= '<hr />';
$code = <<<'EOT'
textarea( 'Textarea' );
EOT;
$form_content_html .= grid( array(
    array( textarea( 'Textarea' ), view_source( $code )),
));


//      Input text appended
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_text( 'Test my appended input', array( 'append' => "After" ));
EOT;
$form_content_html .= grid( array(
    array( input_text( 'Test my appended input', array( 'append' => "After" )), view_source( $code )),
));


//      Input text prepended
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_text( 'Test my prepended input', array( 'prepend' => "Before" ));
EOT;
$form_content_html .= grid( array(
    array( input_text( 'Test my prepended input', array( 'prepend' => "Before" )), view_source( $code )),
));


//      Both
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_text( 'Test both', array( 'prepend' => "+", 'append' => "€" ));
EOT;
$form_content_html .= grid( array(
    array( input_text( 'Test both', array( 'prepend' => "+", 'append' => "€" )), view_source( $code )),
));


//      Select
$form_content_html .= '<hr />';
$code = <<<'EOT'
select( 'Select', array( 'option 1', 'option 2', 'option 3' ));
EOT;
$form_content_html .= grid( array(
    array( select( 'Select', array( 'option 1', 'option 2', 'option 3' )), view_source( $code )),
));

//      Multiple Select
$form_content_html .= '<hr />';
$code = <<<'EOT'
multiple_select( 'Multiple Select', array( 'option 1', 'option 2', 'option 3' ));
EOT;
$form_content_html .= grid( array(
    array( multiple_select( 'Multiple Select', array( 'option 1', 'option 2', 'option 3' )), view_source( $code )),
));

/*
//      Multiple Select
$form_content_html .= '<hr />';
$code = <<<'EOT'
select( 'My Multiple Select', array( 'option 1', 'option 2', 'option 3' ), array( 'select_attributes' => array( 'multiple' => 'multiple' )));
EOT;
$form_content_html .= grid( array(
    array( select( 'My Multiple Select', array( 'option 1', 'option 2', 'option 3' ), array( 'select_attributes' => array( 'multiple' => 'multiple' ))), view_source( $code )),
));
*/

/*
//      Input checkbox
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_checkbox( 'Test my checkbox' );
EOT;
$form_content_html .= grid( array(
    array( input_checkbox( 'Test my checkbox' ), view_source( $code )),
));


//      Input radio
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_radio( 'Test my radio' );
EOT;
$form_content_html .= grid( array(
    array( input_radio( 'Test my radio' ), view_source( $code )),
));
*/

//      Input checkboxes
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_checkboxes( 'Checkboxes Group', array( 'option 1', 'option 2', 'option 3' ));
EOT;
$form_content_html .= grid( array(
    array( input_checkboxes( 'Checkboxes Group', array( 'option 1', 'option 2', 'option 3' )), view_source( $code )),
));


//      Input radios
$form_content_html .= '<hr />';
$code = <<<'EOT'
input_radios( 'Radios Group', array( 'option 1', 'option 2', 'option 3' ));
EOT;
$form_content_html .= grid( array(
    array( input_radios( 'Radios Group', array( 'option 1', 'option 2', 'option 3' )), view_source( $code )),
));



//      Repeatable fields
$form_content_html .= '<hr />';
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



//      File selector
$form_content_html .= '<hr />';
$code = <<<'EOT'
file_selector( 'config', array( 'mask' => '`.*`i' ));
EOT;

$file_selector_html = file_selector( 'config', array( 'mask' => '`.*`i' ));

$form_content_html .= grid( array(
    array( $file_selector_html, view_source( $code )),
));



//      hack 2013/07/24 13:54:28 - since default classes 'span5' to make forms look good in single-column layouts,
//      need to reset those in this mutli-columns layout
$form_content_html = str_replace( 'span5', '', $form_content_html );


//      Render form
$content .= render_form( $form_content_html, '_test_form_submit_handler', array( 'submit_handler_params' => array( 'custom_param' => 1 )));


//      Rendering code source
$content .= '<hr />';
$content .= '<h2>Source code : Form Rendering + Submit handler </h2>';
$code = <<<'EOT'

//      Rendering this form
$content .= render_form( $form_content_html, '_test_form_submit_handler', array( 'submit_handler_params' => array( 'custom_param' => 1 )));

/**
 *  Test form submit callback
 */
function _test_form_submit_handler( $params_arr )
{
    //      debug
    return 'Post : '. kpr( $_POST ) .
        'Params : '. kpr( $params_arr );
}

EOT;
$content .= view_source( $code );



/**
 *  Test form submit callback
 */
function _test_form_submit_handler( $params_arr )
{
    //      debug
    return 'Post : '. kpr( $_POST ) .
        'Params : '. kpr( $params_arr );
}



