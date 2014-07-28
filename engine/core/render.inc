<?php

/**
 *  @file
 *  Templates rendering process
 *  @see route.inc
 */

//      Debug : adding execution time in output
//      (requires content templates printing $content)
if ( $enable_debug && $debug[ 'exec_time' ])
{
    $content .= '<div class="container">';
    $content .= '<small>';
    $content .= get_system_exec_time();
    $content .= '</small>';
    $content .= '</div>';
}

//      debug content_template
if ( $debug[ 'tpl' ])
{
    print '<pre>';
    print "\$content_tpl = $content_tpl";
    print '</pre>';
}

//      Content
ob_start();
include( $content_tpl );
$content_markup .= ob_get_contents();
ob_end_clean();

//      Container (outer wrap)
if ( file_exists( $theme_path .'/'. $content_tpl_type .'.tpl.php' )) {
    $container_tpl = $theme_path .'/'. $content_tpl_type .'.tpl.php';
}

//      debug template_file
if ( $debug[ 'tpl' ])
{
    print '<pre>';
    print "\$container_tpl = $container_tpl";
    print '</pre>';
}

ob_start();
include( $container_tpl );
$final_output = ob_get_contents();
ob_end_clean();

//      Final output
print $final_output;


