<?php

/**
 *  @file
 *  Test : URL / Routing
 *  @path /system/test/routing
 */

//      Init
$head_page_title = "URL / Routing";
$content .= "<p>@see <code>engine/core/api_routing.inc</code></p>";

//      arg
$code = <<<'EOT'
//      get URL argument "à la Drupal"
arg( 0 );
arg( 1 );
arg( 2 );
EOT;
$result_html = 'Result :<br />';
$result_html .= "<p>";
if ( $request_arr[ 0 ] != 'rest' )
    $result_html .= "Useful when inside a REST request (or iframe) : <a href='". url( "rest/". get_current_path()) ."' target='_blank'>see this page in the core REST wrapper</a>";
$result_html .= "</p>";
$result_html .= "<pre>arg( 0 ) = ";
$result_html .= arg( 0 );
$result_html .= "</pre>";
$result_html .= "<pre>arg( 1 ) = ";
$result_html .= arg( 1 );
$result_html .= "</pre>";
$result_html .= "<pre>arg( 2 ) = ";
$result_html .= arg( 2 );
$result_html .= "</pre>";
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      url
$content .= "<hr>";
$code = <<<'EOT'
//      Auto-prefix paths
url( "my/path" );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= url( "my/path" );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      Absolute URL
url( "my/path", array( 'absolute' => true ));
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= url( "my/path", array( 'absolute' => true ));
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      get_current_url
$content .= "<hr>";
$code = <<<'EOT'
//      Get current page URL (default : returns an absolute URL)
get_current_url();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= get_current_url();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      If the current URL had GET params, we can exclude them by calling :
get_current_url( array( 'no_vars' => true ));
EOT;
$result_html = 'Result :<br />';
if ( empty( $_GET[ 'test' ]))
    $result_html .= "<p>Try it with GET params : <a href='". get_current_url() ."?test=1'>see this page's URL with <code>?test=1</code></a></p>";
$result_html .= '<code>';
$result_html .= get_current_url( array( 'no_vars' => true ));
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      If we don't want an absolute URL
get_current_url( array( 'absolute' => false ));
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= get_current_url( array( 'absolute' => false ));
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      get_current_path
$content .= "<hr>";
$code = <<<'EOT'
//      Get current path
get_current_path();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= get_current_path();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      Get current path with base path
get_current_path( array( 'with_base_path' => true ));
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= get_current_path( array( 'with_base_path' => true ));
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      reload_link
$content .= "<hr>";
$code = <<<'EOT'
//      Simple reload page link
//      (not "refresh", just the exact same link of current page)
reload_link();
EOT;
$result_html = 'Result :<br />';
$result_html .= reload_link();
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      Specify label
reload_link( "Try again" );
EOT;
$result_html = 'Result :<br />';
$result_html .= reload_link( "Try again" );
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));



//      Notes

$content .= "<hr>";
$content .= "<h3>Notes</h3>";
$content .= <<<'EOT'
<pre>
Mini HTTP request engine (from scratch)

Defines context based on URL,
Includes context-specific process code,
Handles theming based on context-specific templates.

@example    http://www.url.com?q=section/param/test
            rewritten : http://www.url.com/section/param/test
            context-specific process files looked for in '/context' dir (all files found are loaded):
            => section_param_test.inc
            => section_param.inc
            => section.inc
            template files looked for in '/theme' dir (only the most specific - the first match - is used) :
            => page.tpl.php + section-param-test.tpl.php
            => page.tpl.php + section-param.tpl.php
            => page.tpl.php + section.tpl.php
            => page.tpl.php + default.tpl.php (when nothing else matches)

@example    http://www.url.com?q=rest/request-name/param/test
            rewritten : http://www.url.com/rest/request-name/param/test
            template files looked for in '/theme' dir (only the most specific - the first match - is used) :
            => rest.tpl.php + rest-request-name-param-test.tpl.php
            => rest.tpl.php + rest-request-name-param.tpl.php
            => rest.tpl.php + rest-request-name.tpl.php
            => rest.tpl.php + request-name-param-test.tpl.php
            => rest.tpl.php + request-name-param.tpl.php
            => rest.tpl.php + request-name.tpl.php
            => rest.tpl.php + raw.tpl.php (when nothing else matches)

@example    http://www.url.com?q=iframe/section/param/test
            rewritten : http://www.url.com/iframe/section/param/test
            template files looked for in '/theme' dir (only the most specific - the first match - is used) :
            => iframe.tpl.php + iframe-section-param-test.tpl.php
            => iframe.tpl.php + iframe-section-param.tpl.php
            => iframe.tpl.php + iframe-section.tpl.php
            => iframe.tpl.php + section-param-test.tpl.php
            => iframe.tpl.php + section-param.tpl.php
            => iframe.tpl.php + section.tpl.php
            => iframe.tpl.php + default.tpl.php (when nothing else matches)
</pre>
EOT;

