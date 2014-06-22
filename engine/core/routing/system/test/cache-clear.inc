<?php

/**
 *  @file
 *  Test : cache clear
 *  @path /sytem/test/cache-clear
 */

//      Init
$head_page_title = "Test cache clear";

//      Write results
cache_set( 'TestCacheClear', array( "bla" ));

$code = <<<'EOT'
//      Setup a temp cache to clear
cache_set( 'TestCacheClear', array( "bla" ));

//      Get link to clear cache
$content .= cache_clear_link( 'TestCacheClear' );
EOT;

//      Link to delete
$content .= grid( array(
    array(
        "<h2>Snippet</h2><p>(Code used for test)</p>",
        "<h2>Result</h2>",
    ),
    array(
        view_source( $code ),
        cache_clear_link( 'TestCacheClear' ),
    ),
));


