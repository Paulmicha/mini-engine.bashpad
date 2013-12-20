<?php

/**
 *  @file
 *  Test : File utilities
 *  @path /system/test/file
 */

//      Init
$head_page_title = "File utilities";
$content .= "<p>@see <code>engine/core/helpers_file.inc</code></p>";
$content .= "<p>Lists files recursively in all subdirs</p>";

//      dirname( __FILE__ )
$code = <<<'EOT'
//      Current dir test
$files = list_files( dirname( __FILE__ ));
EOT;

$files = list_files( dirname( __FILE__ ));
if ( is_string( $files ))
    $res = $files;
else
    $res = kpr( $files, 1 );

$result_html = 'Result :<br />';
$result_html .= $res;
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      Tree
$content .= "<hr>";
$code = <<<'EOT'
//      This allows to fetch files as a "recursive" array tree
//      Note : we should hit this application "root" dir
$files = list_files( dirname( __FILE__ ) .'/../../../', array( 'tree' => true ));
EOT;
$files = list_files( dirname( __FILE__ ) .'/../../../', array( 'tree' => true ));
if ( is_string( $files ))
    $res = $files;
else
    $res = kpr( $files, 1 );

$result_html = '<i class="icon-thumbs-up-alt icon-large"></i>&nbsp;Result :<br />';
$result_html .= $res;
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      Max depth
$content .= "<hr>";
$code = <<<'EOT'
//      Get only the files inside the application "root" dir
$files = list_files( dirname( __FILE__ ) .'/../../../', array(
    'max_depth' => 0
));
EOT;
$files = list_files( dirname( __FILE__ ) .'/../../../', array(
    'max_depth' => 0,
));
if ( is_string( $files ))
    $res = $files;
else
    $res = kpr( $files, 1 );

$result_html = 'Result :<br />';
$result_html .= $res;
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      Filter just ".js" files
$content .= "<hr>";
$code = <<<'EOT'
//      Only list Javascript files ('*.js') in the whole projet
//      Note : 'mask' is a pattern for preg_match(),
//      allowing to include only matching filenames
$files = list_files( dirname( __FILE__ ) .'/../../../', array( 'mask' => '`.\.js`i' ));
EOT;
$files = list_files( dirname( __FILE__ ) .'/../../../', array( 'mask' => '`.\.js`i' ));
if ( is_string( $files ))
    $res = $files;
else
    $res = kpr( $files, 1 );

$result_html = 'Result :<br />';
$result_html .= $res;
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      Doc
$content .= "<hr>";
$code = <<<'EOT'
//      Here are the defaults for list_files()
//      Note the 'max_items' and 'max_depth', which need to be overridden if needed
array(
    //'order' => RecursiveIteratorIterator::LEAVES_ONLY, //  (default)
    //'order' => RecursiveIteratorIterator::SELF_FIRST,
    'order' => RecursiveIteratorIterator::CHILD_FIRST,
    'mask' => '',
    'exclusions' => array( '.', '..' ),
    'tree' => false,
    'max_items' => 9999,
    'max_depth' => 999,
    'files_only' => true,
    'dirs_only' => false,
);
EOT;
$content .= view_source( $code );


