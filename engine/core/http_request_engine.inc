<?php

/**
 *  @file
 *  Mini HTTP request engine (routing from scratch)
 *  
 *  Defines route based on URL,
 *  Includes route-specific process code,
 *  Handles theming based on route-specific templates.
 *  
 *  NB: Characters allowed in URLs (before '?') are :
 *      alphanumeric lowercase
 *      slashes: /
 *      hyphen: -
 *  
 *  @example    http://www.example.com?q=section/param/test
 *              rewritten : http://www.example.com/section/param/test
 *              routing-specific process files looked for (all files found are loaded):
 *              => routing/section_param_test.inc
 *              => routing/section_param.inc
 *              => routing/section.inc
 *              template files looked for (only the most specific - the first match - is used) :
 *              => page.tpl.php + section-param-test.tpl.php
 *              => page.tpl.php + section-param.tpl.php
 *              => page.tpl.php + section.tpl.php
 *              => page.tpl.php + default.tpl.php (when nothing else matches)
 *
 *  @example    http://www.example.com?q=rest/request-name/param/test
 *              rewritten : http://www.example.com/rest/request-name/param/test
 *              template files looked for (only the most specific - the first match - is used) :
 *              => rest.tpl.php + rest-request-name-param-test.tpl.php
 *              => rest.tpl.php + rest-request-name-param.tpl.php
 *              => rest.tpl.php + rest-request-name.tpl.php
 *              => rest.tpl.php + request-name-param-test.tpl.php
 *              => rest.tpl.php + request-name-param.tpl.php
 *              => rest.tpl.php + request-name.tpl.php
 *              => rest.tpl.php + raw.tpl.php (when nothing else matches)
 *
 *  @example    http://www.example.com?q=iframe/section/param/test
 *              rewritten : http://www.example.com/iframe/section/param/test
 *              template files looked for (only the most specific - the first match - is used) :
 *              => iframe.tpl.php + iframe-section-param-test.tpl.php
 *              => iframe.tpl.php + iframe-section-param.tpl.php
 *              => iframe.tpl.php + iframe-section.tpl.php
 *              => iframe.tpl.php + section-param-test.tpl.php
 *              => iframe.tpl.php + section-param.tpl.php
 *              => iframe.tpl.php + section.tpl.php
 *              => iframe.tpl.php + default.tpl.php (when nothing else matches)
 */


//      Homepage
$arg0 = arg( 0 );
if ( empty( $arg0 ))
{
    $routing_handler_file = $routing_handlers_path .'/'. $content_include .".inc";
    
    if ( file_exists( $routing_handler_file ))
    {
        include $routing_handler_file;
        
        //      debug
        if ( !empty( $debug[ 'routing' ]))
        {
            print '<pre>';
            print "$routing_handler_file loaded.";
            print '</pre>';
        }
    }
}

//      First valid request type found is selected
else
{
    //      404 handler
    $is_404 = true;
    
    //      Handle candidates so that the most specific can be used first
    $request_arr = get_request_arr();
    
    //      Include context-specific process code
    $context_handlers_arr = $request_arr;
    
    //      modif 2013/03/21 18:21:29 - in descending order (top > bottom, or container > contained)
    $prev_ch = '';
    foreach( $context_handlers_arr as $i => $ch )
    {
        if ( !empty( $prev_ch ))
            $routing_handler_file = $prev_ch ."_". $ch .".inc";
        else
            $routing_handler_file = $routing_handlers_path .'/'. $ch .".inc";
        $prev_ch = str_replace( '.inc', '', $routing_handler_file );
        
        if ( file_exists( $routing_handler_file ))
        {
            include $routing_handler_file;
            $is_404 = false;
            
            //      debug
            if ( !empty( $debug[ 'routing' ]))
            {
                print '<pre>';
                print "$routing_handler_file loaded.";
                print '</pre>';
            }
        }
        
        //      debug
        else if ( !empty( $debug[ 'routing' ]))
        {
            print '<pre>';
            print "($routing_handler_file not found)";
            print '</pre>';
        }
    }
    
    //      update 2013/03/25 12:40:00 - 2nd pass for loading .inc during REST OR iframes requests
    if ( in_array( $request_arr[ 0 ], array( 'rest', 'iframe' )))
    {
        $stemmed_context_handlers_arr = $request_arr;
        array_shift( $stemmed_context_handlers_arr );
        $prev_ch = '';
        foreach( $stemmed_context_handlers_arr as $i => $ch )
        {
            if ( !empty( $prev_ch ))
                $routing_handler_file = $prev_ch ."_". $ch .".inc";
            else
                $routing_handler_file = $routing_handlers_path .'/'. $ch .".inc";
            $prev_ch = str_replace( '.inc', '', $routing_handler_file );
            
            if ( file_exists( $routing_handler_file ))
            {
                include $routing_handler_file;
                $is_404 = false;
                
                //      debug
                if ( !empty( $debug[ 'routing' ]))
                {
                    print '<pre>';
                    print "$routing_handler_file loaded.";
                    print '</pre>';
                }
            }
            
            //      debug
            else if ( !empty( $debug[ 'routing' ]))
            {
                print '<pre>';
                print "($routing_handler_file not found)";
                print '</pre>';
            }
        }
    }
    
    //      Theming - determine content template file to use (wrapper)
    $tpl_suggestions_arr = $request_arr;
    $tpl_suggestion = $theme_path .'/'. join( "-", $tpl_suggestions_arr ) .".tpl.php";
    $most_specific_tpl_suggestion_matched = false;
    while ( !empty( $tpl_suggestions_arr ))
    {
        if ( file_exists( $tpl_suggestion ))
        {
            $content_template = $tpl_suggestion;
            $is_404 = false;
            
            //      debug
            if ( !empty( $debug[ 'routing' ]))
            {
                print '<pre>';
                print "$tpl_suggestion CHECKS";
                print '</pre>';
            }
            
            //      hack 2013/03/25 15:25:39 - avoid 2nd pass when most specific match is reached
            if ( count( $tpl_suggestions_arr ) > 1 )
                $most_specific_tpl_suggestion_matched = true;
            
            break;
        }
        
        //      debug
        if ( !empty( $debug[ 'routing' ]))
        {
            print '<pre>';
            print "($tpl_suggestion not found)";
            print '</pre>';
        }
        
        array_pop( $tpl_suggestions_arr );
        $tpl_suggestion = $theme_path .'/'. join( "-", $tpl_suggestions_arr ) .".tpl.php";
    }
    
    //      Main container template (content)
    $run_2nd_pass = false;
    
    switch ( $request_arr[ 0 ])
    {
        case 'rest':
            $content_template_type = 'rest';
            if ( !$most_specific_tpl_suggestion_matched )
                $run_2nd_pass = true;
            array_shift( $request_arr );
        break;
        case 'iframe':
            $content_template_type = 'iframe';
            if ( !$most_specific_tpl_suggestion_matched )
                $run_2nd_pass = true;
            array_shift( $request_arr );
        break;
    }
    
    //      Load inside each possible "wrapper" template file, the "normal" candidate, lowest-specificity support (defaults)
    if ( $run_2nd_pass )
    {
        //      2nd pass for Theming : content template file to use
        $tpl_suggestions_arr = $request_arr;
        $tpl_suggestion = $theme_path .'/'. join( "-", $tpl_suggestions_arr ) .".tpl.php";
        while ( !empty( $tpl_suggestions_arr ))
        {
            if ( file_exists( $tpl_suggestion ))
            {
                $content_template = $tpl_suggestion;
                $is_404 = false;
                break;
            }
            array_pop( $tpl_suggestions_arr );
            $tpl_suggestion = $theme_path .'/'. join( "-", $tpl_suggestions_arr ) .".tpl.php";
        }
        
        //      Provide default template (in case none exists to match current path)
        if ( $tpl_suggestion == $theme_path .'/.tpl.php' && file_exists( $theme_path .'/raw.tpl.php' ))
            $content_template = $theme_path .'/raw.tpl.php';
    }
    
    //      404 handler
    if ( $is_404 ) {
        send_404();
    }
    
    //      modif 2013/03/21 15:14:42 - si on arrive sur l'index alors que $_REQUEST n'est pas vide, on prend le default s'il existe
    elseif ( $content_template == $theme_path .'/index.tpl.php' && file_exists( $theme_path .'/default.tpl.php' ))
        $content_template = $theme_path .'/default.tpl.php';
}


