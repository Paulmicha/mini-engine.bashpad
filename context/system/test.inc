<?php

/**
 *  @file
 *  Tests (dispatcher file)
 *  @path system/test/<namespace>
 */

//      Init
$head_page_title = "System - Tests";
$namespace = check_plain( arg( 2 ));

//      Failsafe
if ( empty( $namespace ))
    $content .= alert( "Missing 'namespace' as argument" );
else
{
    //      Including corresponding test file
    include "context/system/test/$namespace.inc";
}


//-------------------------------------------------------------------------------------
//      Helpers


/**
 *  Render tests from array
 */
function _render_tests( $tests, $options = array())
{
    $html = '';
    if ( is_array( $tests ))
    {
        $html .= '<ol>';
        foreach( $tests as $test )
        {
            //      debug
            //$html .= '<pre>debug = ';
            //$html .= print_r( $test, 1 );
            //$html .= '</pre>';
            
            $html .= '<li>';
            $html .= $test[ 'desc' ];
            
            //$html .= '<br />';
            $html .= ' : ';
            
            if ( $test[ 'result' ])
                $html .= '<span class="test-result pass" style="color:limegreen"><i class="icon-ok"></i>Pass</span>';
            else
                $html .= '<span class="test-result fail" style="color:red"><i class="icon-remove"></i>Fail</span>';
            
            if ( !empty( $test[ 'debug' ]))
            {
                $html .= '<br />';
                $html .= '<pre>';
                $html .= $test[ 'debug' ];
                $html .= '</pre>';
            }
            
            $html .= '</li>';
        }
        $html .= '</ol>';
    }
    return $html;
}



