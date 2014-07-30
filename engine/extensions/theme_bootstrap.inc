<?php

/**
 *  @file
 *  Front-end (theme) : Bootstrap + variant
 *  @see http://bootswatch.com
 */

//      Bootstrap JS (all)
add_js( 'theme/_assets/vendor/bootstrap/'. $extensions[ 'theme_bootstrap' ][ 'settings' ][ 'v' ] .'/js/bootstrap.min.js' );


//      Custom Google Font (JS method)
//      -> using customized Bootswatch CSS (stripped of default font-face)
add_css( 'theme/_assets/vendor/mini-engine/'. $mini_engine_version
    .'/css/bootswatch-'
    . $extensions[ 'theme_bootstrap' ][ 'settings' ][ 'v' ]
    .'--'
    . $extensions[ 'theme_bootstrap' ][ 'settings' ][ 'bootswatch' ]
    .'--nofont.css' );

$js_inline = <<<EOT
  WebFontConfig = {
    google: { families: [ 'Open+Sans:300italic,400italic,700italic,400,700,300:latin' ] }
  };
  (function() {
    var wf = document.createElement('script');
    wf.src = ('https:' == document.location.protocol ? 'https' : 'http') +
      '://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';
    wf.type = 'text/javascript';
    wf.async = 'true';
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(wf, s);
  })();
EOT;

add_js( $js_inline, array( 'inline' => true ));


//      Bootstrap 2.x specifics
if ( $extensions[ 'theme_bootstrap' ][ 'settings' ][ 'v' ] < '3.0.0' )
{
    //      Font-awesome 3.x
    add_css( 'theme/_assets/vendor/font-awesome/3.2.1/css/font-awesome.min.css' );
    $head_extra_markup .= '
    <!--[if lt IE 8]>
    <link rel="stylesheet" href="'. url( 'theme/_assets/vendor/font-awesome/3.2.1/css/font-awesome.min.css' ) .'">
    <![endif]-->
    ';
    
    //      Back then, responsive was opt-in
    add_css( 'theme/_assets/vendor/bootstrap/'
        . $extensions[ 'theme_bootstrap' ][ 'settings' ][ 'v' ]
        .'/css/bootstrap-responsive.min.css' );
}


