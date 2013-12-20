<?php

/**
 *  @file
 *  Test : cache
 *  @path /system/test/cache
 */

//      Init
$head_page_title = "Test cache";
$tests = array();

//      Writes : string
$tests[] = array(
    'desc' => "Testing cache writing : string",
    'result' => is_numeric( cache_set( 'testA', "bla" )),
);
//      Reads : string
$tests[] = array(
    'desc' => "Testing cache reading : string",
    'result' => ( cache_get( 'testA' ) == 'bla' ),
);
//      Delete : string
$tests[] = array(
    'desc' => "Testing cache deletion : string",
    'result' => ( cache_clear( 'testA' )),
);

//      Writes : array
$arr = array( "bla" => "blo", 3 => "accent éàç" );
$tests[] = array(
    'desc' => "Testing cache writing : array",
    'result' => is_numeric( cache_set( 'testB', $arr )),
    //'debug' => print_r( $arr, 1 ),
);
//      Reads : array
$test_b_arr = cache_get( 'testB' );
$tests[] = array(
    'desc' => "Testing cache reading : array",
    'result' => ( is_array( $test_b_arr ) && $test_b_arr[ 'bla' ] == 'blo' && $test_b_arr[ 3 ] == 'accent éàç' ),
    //'debug' => print_r( $test_b_arr, 1 ),
);
//      Delete : array
$tests[] = array(
    'desc' => "Testing cache deletion : array",
    'result' => ( cache_clear( 'testB' )),
);


//      Write results
//$content .= _render_tests( $tests );


//      Snippets
$snippets_html = '';

//      Writes : string
$snippets_html .= '<h4>1. Writes : string</h4>';
$code = <<<'EOT'
//      This returns Integer : last inserted id
$inserted_id = cache_set( 'testA', "bla" );
EOT;
$snippets_html .= view_source( $code );

//      Reads : string
$snippets_html .= '<h4>2. Reads : string</h4>';
$code = <<<'EOT'
//      This returns (mixed) : cached value
$value = cache_get( 'testA' );
EOT;
$snippets_html .= view_source( $code );

//      Delete : string
$snippets_html .= '<h4>3. Delete</h4>';
$code = <<<'EOT'
//      This returns Boolean : success flag
$success_flag = cache_clear( 'testA' );
EOT;
$snippets_html .= view_source( $code );

//      Writes : array
$snippets_html .= '<h4>4. Writes : Array</h4>';
$code = <<<'EOT'
//      This returns Integer : last inserted id
$arr = array( "bla" => "blo", 3 => "accent éàç" );
$inserted_id = cache_set( 'testB', $arr );
EOT;
$snippets_html .= view_source( $code );

//      Reads : array
$snippets_html .= '<h4>5. Reads : Array</h4>';
$code = <<<'EOT'
//      This returns (mixed) : cached value
$test_b_arr = cache_get( 'testB' );
EOT;
$snippets_html .= view_source( $code );
$snippets_html .= '<p>Résultat : </p>';
$snippets_html .= view_source( '//      Pour info, on debug avec :
print kpr( $test_b_arr );' );
$snippets_html .= kpr( $test_b_arr );




//      Render
$content .= grid( array(
    array(  "<h2>Snippets</h2><p>(Code used for each test)</p>", "<h2>Tests</h2>" ),
    array( $snippets_html, _render_tests( $tests )),
));





