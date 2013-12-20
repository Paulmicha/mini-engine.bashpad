<?php

/**
 *  @file
 *  Theming rendering process
 */

//      debug content_template
if ( $GLOBALS[ 'debug' ] && $GLOBALS[ 'debug' ][ 'routing' ])
{
    print '<pre>';
    print "content_template = $content_template";
    print '</pre>';
}

//      Content
//try
//{
    ob_start();
    include( $content_template );
    $content_markup .= ob_get_contents();
    ob_end_clean();
//}
//catch( Exception $e ) {
//    $content_markup .= '<pre class="error">'. $e .'</pre>';
//}

//      Container
if ( file_exists( $theme_path .'/'. $content_template_type .'.tpl.php' )) {
    $template_file = $theme_path .'/'. $content_template_type .'.tpl.php';
}

//      debug template_file
if ( $GLOBALS[ 'debug' ] && $GLOBALS[ 'debug' ][ 'routing' ])
{
    print '<pre>';
    print "template_file = $template_file";
    print '</pre>';
}

//try
//{
    ob_start();
    include( $template_file );
    $output = ob_get_contents();
    ob_end_clean();
//}
//catch( Exception $e ) {
//    $content_markup .= '<pre class="error">'. $e .'</pre>';
//}

//      Final output
print $output;


