<?php

/**
 *  @file
 *  Test : Various utilities
 *  @path /system/test/utilities
 */

//      Init
$head_page_title = "Utilities";


//--------------------------------------------------------------------
//      Serialize

$content .= "<h3>Serialization</h3>";
$content .= "<p>@see <code>engine/core/helpers_str.inc</code></p>";

//      safe_serialize
$code = <<<'EOT'
//      Serialize safely
//      Avoids problems with accents in serialized strings stored in DB as BLOB
$data = array( 'test' => 'Accents éàùç $£€ test' );
$serialized_str = safe_serialize( $data );

//      Read
$unserialized_data = safe_unserialize( $serialized_str );

//      Test
if ( $unserialized_data[ 'test' ] === $data[ 'test' ])
    $content .= alert( "Pass", 'success' );
else
    $content .= alert( "Oops", 'error' );
EOT;
$data = array( 'test' => 'Accents éàùç $£€ test' );
$serialized_str = safe_serialize( $data );
$unserialized_data = safe_unserialize( $serialized_str );
$result_html = 'Result :<br />';
if ( $unserialized_data[ 'test' ] === $data[ 'test' ])
    $result_html .= alert( "Pass", 'success' );
else
    $result_html .= alert( "Oops", 'error' );
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));



//--------------------------------------------------------------------
//      String

$content .= "<hr>";
$content .= "<h3>String</h3>";
$content .= "<p>These are the most common, more in <code>engine/core/helpers_str.inc</code></p>";


//      str_clean
$code = <<<'EOT'
//      "Machinification" of any string
//      could be used as URL 'slug'
str_clean( "It's a wonderful night" );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= str_clean( "It's a wonderful night" );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      2nd param is the "replace by" character
str_clean( "Accents éàçùÉÀÇÙ%^$£€î test", '_' );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= str_clean( 'Accents éàçùÉÀÇÙ%^$£€î test', '_' );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      user_password
$content .= "<hr>";
$code = <<<'EOT'
//      Simple random string generation (e.g. for passwords)
user_password();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= user_password();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      Length
user_password( 55 );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= user_password( 55 );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      strip_line_breaks
$content .= "<hr>";
$code = <<<'EOT'
//      Remove line breaks
strip_line_breaks( "My string
has two line
breaks." );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<pre>';
$result_html .= strip_line_breaks( "My string
has two line
breaks." );
$result_html .= '</pre>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));
$content .= "<hr>";
$code = <<<'EOT'
//      Remove line breaks and replace by '|'
strip_line_breaks( "My string
has two line
breaks.", '|' );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<pre>';
$result_html .= strip_line_breaks( "My string
has two line
breaks.", '|' );
$result_html .= '</pre>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//      check_plain
$content .= "<hr>";
$code = <<<'EOT'
//      Encodes special characters in a plain-text string for display as HTML.
//      Also validates strings as UTF-8 to prevent cross site scripting attacks on Internet Explorer 6.
//      @see http://api.drupal.org/api/drupal/includes%21bootstrap.inc/function/check_plain/7
$str = check_plain( "<script type='text/javascript'>alert( 'test' );</script>" );

//      Debug
$content .= kpr( $str );
EOT;
$content .= grid( array(
    array( view_source( $code ), kpr( check_plain( "<script type='text/javascript'>alert( 'test' );</script>" ))),
));



//--------------------------------------------------------------------
//      Debug


$content .= "<hr>";
$content .= "<h3>Debug</h3>";
$content .= "<p>@see <code>engine/core/helpers_system.inc</code></p>";

$code = <<<'EOT'
$obj = new stdClass();
$obj->test = array( 1, 'a' => "b" );
$data = array(
    $obj,
    'test' => &$obj,
) + array( 3 );

//      Debug
$content .= kpr( $data );

//      Debug alternative
//      (writes in the output buffer directly, same as calling 'print')
krumo( $data );
EOT;
$result_html = 'Result :<br />';
$obj = new stdClass();
$obj->test = array( 1, 'a' => "b" );
$data = array(
    $obj,
    'test' => &$obj,
) + array( 3 );
$result_html .= kpr( $data );
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));


//--------------------------------------------------------------------
//      Date

$content .= "<hr>";
$content .= "<h3>Date & Time</h3>";
$content .= "<p>@see <code>engine/core/helpers_date.inc</code></p>";

//      dtstmpstr
$content .= "<hr>";
$code = <<<'EOT'
//      Reference datestamp - string for date
dtstmpstr();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= dtstmpstr();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      datestamp
$content .= "<hr>";
$code = <<<'EOT'
//      Reference datestamp (BIGINT) - to seconds
datestamp();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= datestamp();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      timestamp_format
$content .= "<hr>";
$code = <<<'EOT'
//      Quick date formatter
timestamp_format();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= timestamp_format();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      timestamp_format
$content .= "<hr>";
$code = <<<'EOT'
//      Quick date formatter - 1st arg is a UNIX timestamp from time()
//      2nd arg string for date format
$time = time() - 60 * 60;
timestamp_format( $time, "H:i:s" );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$time = time() - 60 * 60;
$result_html .= timestamp_format( $time, "H:i:s" );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      datestamp_format
$content .= "<hr>";
$code = <<<'EOT'
//      Quick date formatter
datestamp_format();
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$result_html .= datestamp_format();
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      datestamp_format
$content .= "<hr>";
$code = <<<'EOT'
//      Quick date formatter - 1st arg is for strtotime()
//      2nd arg string for date format
$datestamp = "+1 week";
datestamp_format( $datestamp, "Y-m-d" );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$datestamp = "+1 week";
$result_html .= datestamp_format( $datestamp, "Y-m-d" );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      datetime_period_active
$content .= "<hr>";
$code = <<<'EOT'
//      Test if a period is active
$datestamp_start = date( dtstmpstr(), strtotime( '-1 week' ));
$datestamp_stop = date( dtstmpstr(), strtotime( '+1 week' ));
datetime_period_active( $datestamp_start, $datestamp_stop );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$datestamp_start = date( dtstmpstr(), strtotime( '-1 week' ));
$datestamp_stop = date( dtstmpstr(), strtotime( '+1 week' ));
$result_html .= datetime_period_active( $datestamp_start, $datestamp_stop );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      datetime_period_active
$content .= "<hr>";
$code = <<<'EOT'
//      3rd param is to get "too soon" or "too late",
//      instead of just Boolean FALSE
$datestamp_start = date( dtstmpstr(), strtotime( '-1 week' ));
$datestamp_stop = date( dtstmpstr(), strtotime( '-1 day' ));
datetime_period_active( $datestamp_start, $datestamp_stop, true );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$datestamp_start = date( dtstmpstr(), strtotime( '-1 week' ));
$datestamp_stop = date( dtstmpstr(), strtotime( '-1 day' ));
$result_html .= datetime_period_active( $datestamp_start, $datestamp_stop, true );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));

//      datetime_period_active
$content .= "<hr>";
$code = <<<'EOT'
//      Last test
$datestamp_start = date( dtstmpstr(), strtotime( '+1 day' ));
$datestamp_stop = date( dtstmpstr(), strtotime( '+1 week' ));
datetime_period_active( $datestamp_start, $datestamp_stop, true );
EOT;
$result_html = 'Result :<br />';
$result_html .= '<code>';
$datestamp_start = date( dtstmpstr(), strtotime( '+1 day' ));
$datestamp_stop = date( dtstmpstr(), strtotime( '+1 week' ));
$result_html .= datetime_period_active( $datestamp_start, $datestamp_stop, true );
$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));



//--------------------------------------------------------------------
//      Array


$content .= "<hr>";
$content .= "<h3>Array</h3>";
$content .= "<p>@see <code>engine/core/helpers_array.inc</code></p>";

$code = <<<'EOT'
//      Recursively merge arrays :
//      overrides 1st array's keys if collision
$arr1 = array(
    'A' => "1.A",
    'B' => array(
        'BA' => "1.BA",
        'BB' => "1.BB",
    ),
    'C' => array(
        'CA' => "1.CA",
    ),
);
$arr2 = array(
    'A' => "2.A",
    'B' => array(
        'BB' => "2.BB",
        'BC' => "2.BC",
    ),
    'C' => array(
        'CB' => "2.CB",
    ),
);
$merged_arr = array_merge_recursive_overwrite( $arr1, $arr2 );
EOT;
$arr1 = array(
    'A' => "1.A",
    'B' => array(
        'BA' => "1.BA",
        'BB' => "1.BB",
    ),
    'C' => array(
        'CA' => "1.CA",
    ),
);
$arr2 = array(
    'A' => "2.A",
    'B' => array(
        'BB' => "2.BB",
        'BC' => "2.BC",
    ),
    'C' => array(
        'CB' => "2.CB",
    ),
);
$content .= grid( array(
    //array( view_source( $code ), kpr( array_merge_recursive_overwrite( $arr1, $arr2 ))),
    array( view_source( $code ), view_source( print_r( array_merge_recursive_overwrite( $arr1, $arr2 ), 1 ))),
));



