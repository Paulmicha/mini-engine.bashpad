<?php

/**
 *  @file
 *  System UI - custom CSS
 */

$css_inline = <<<EOT
    
    body { padding:10px 0 30px 0; }
    h1 a { font-size:75%; }
    h1 + .nav { margin-top:2em; }
    .nav > li > small { padding: 11px 10px 8px; display:block; }

    .krumo-footnote { border:0 none; }
    .krumo-footnote .krumo-version { display:none; }

    .repeatable-fields-list .control-group { margin-bottom:8px; }

    body div.krumo-root { border:0 none; }

    .field-group-inner-wrap + .field-group-inner-wrap { margin-top:1em; }
    .multifield-wrap + .multifield-wrap { margin-top:1em; }

    .repeatable-wrap .field-group-inner-wrap { border:1px dotted #999; padding:1em; -webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px; }

    .alert-info a { background:#FFF; padding:.2em; }

    /*      Form - Component - File selector "tree"
    */
    .file-selector-tree { margin: 0; }
    .file-selector-tree,
    .file-selector-tree ul { list-style:none; }
    
EOT;
add_css( $css_inline, array( 'inline' => true ));


//      Syntax highlighting
add_css( 'theme/_assets/vendor/highlightjs/8.1/styles/codepen-embed.css' );
add_js( 'theme/_assets/vendor/highlightjs/8.1/highlight.pack.js' );
add_js( 'hljs.initHighlightingOnLoad();', array( 'inline' => true ));

