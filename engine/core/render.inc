<?php

/**
 *  @file
 *  Templates rendering process
 *  @see engine/core/router.inc
 */

//      debug
if ( $debug[ 'tpl' ])
{
    print '<pre>';
    print "\$content_tpl = $content_tpl";
    print '</pre>';
}

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

//      Last chance to alter content before rendering
hook( 'preprocess_render' );

//      Content
$content_rendered .= render_tpl( $content_tpl );

//      Container (outer wrap)
if ( file_exists( $theme_path .'/'. $base_tpl .'.tpl.php' )) {
    $container_tpl = $theme_path .'/'. $base_tpl .'.tpl.php';
}

//      debug
if ( $debug[ 'tpl' ])
{
    print '<pre>';
    print "\$container_tpl = $container_tpl";
    print '</pre>';
}

//      Final output
print render_tpl( $container_tpl );


