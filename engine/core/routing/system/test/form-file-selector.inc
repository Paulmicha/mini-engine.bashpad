<?php

/**
 *  @file
 *  Test : form - File Selector
 *  @path /system/test/form-file-selector
 */

//      Init
$form_content_html = '';
$head_page_title = "Test form - File Selector";
$form_content_html .= grid( array(
    array(  "<h2>Result</h2>", "<h2>Snippet</h2>" ),
));

//      File selector
$code = <<<'EOT'
file_selector( 'config', array( 'mask' => '`.*`i' ));
EOT;

$file_selector_html = file_selector( 'config', array( 'mask' => '`.*`i' ));

$form_content_html .= grid( array(
    array( $file_selector_html, view_source( $code )),
));


//      Render form
$content .= $form_content_html;


