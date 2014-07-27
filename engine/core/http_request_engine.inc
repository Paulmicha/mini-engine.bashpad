<?php

/**
 *  @file
 *  Mini HTTP request engine (minimal routing + templating from scratch)
 *  
 *  This "routing & templating" mechanism works by checking if certain files exist,
 *  and includes them, or sends a 404 if no files correspond to the current URL.
 *  
 *  For instance, this URL http://example.com?q=section/param/test (rewritten : http://example.com/section/param-name/test)
 *  will have this corresponding "request" : [ 'section', 'param-name', 'test' ].
 *  See corresponding candidate files examples below.
 *  
 *  NB: Characters allowed in URLs (parts separated by "/") are :
 *      alphanumeric lowercase
 *      slashes: /
 *      hyphen: -
 *      dots: .
 *  
 *  Dirs (relative to project dir) are configurable in settings, or even on-the-fly.
 *  The examples below use the defaults :
 *      $routing_handlers_path = 'routing'
 *      $theme_path = 'theme'
 *  
 *  There are 2 types of templates : "outer wrapper", and "content" templates.
 *  Extensions supported are ".tpl.php" and ".html" for content templates.
 *  2 Special "route" types - beginning with 'rest' or 'iframe' :
 *      • both use different "outer wrapper" templates, designed for their specific needs
 *      • see last 2 examples below for special extra matching rules for content templates in these cases
 *  
 *  @example    http://example.com?q=section/param/test
 *              rewritten : http://example.com/section/param-name/test
 *              • routing-specific process files looked for (all files found are loaded):
 *              => routing/section/param-name/test.inc
 *              => routing/section/param-name.inc
 *              => routing/section.inc
 *              • template files looked for (only the most specific - the first match - is used) :
 *              => theme/page.tpl.php + theme/section/param-name/test.tpl.php
 *              => theme/page.tpl.php + theme/section/param-name.tpl.php
 *              => theme/page.tpl.php + theme/section.tpl.php
 *              => theme/page.tpl.php + theme/default.tpl.php (when nothing else matches)
 *  
 *  @example    http://example.com?q=rest/request-name/param/test
 *              rewritten : http://example.com/rest/request-name/param/test
 *              • template files looked for (only the most specific - the first match - is used) :
 *              => theme/rest.tpl.php + theme/rest/request-name/param/test.tpl.php
 *              => theme/rest.tpl.php + theme/rest/request-name/param.tpl.php
 *              => theme/rest.tpl.php + theme/rest/request-name.tpl.php
 *              => theme/rest.tpl.php + theme/request-name/param/test.tpl.php
 *              => theme/rest.tpl.php + theme/request-name/param.tpl.php
 *              => theme/rest.tpl.php + theme/request-name.tpl.php
 *              => theme/rest.tpl.php + theme/raw.tpl.php (when nothing else matches)
 *  
 *  @example    http://example.com?q=iframe/section/param/test
 *              rewritten : http://example.com/iframe/section/param/test
 *              • template files looked for (only the most specific - the first match - is used) :
 *              => theme/iframe.tpl.php + theme/iframe/section/param/test.tpl.php
 *              => theme/iframe.tpl.php + theme/iframe/section/param.tpl.php
 *              => theme/iframe.tpl.php + theme/iframe/section.tpl.php
 *              => theme/iframe.tpl.php + theme/section/param/test.tpl.php
 *              => theme/iframe.tpl.php + theme/section/param.tpl.php
 *              => theme/iframe.tpl.php + theme/section.tpl.php
 *              => theme/iframe.tpl.php + theme/default.tpl.php (when nothing else matches)
 *  
 *  @see get_request_arr() in api_routing.inc
 */


//      Homepage
$arg0 = arg( 0 );
if ( empty( $arg0 ))
{
    $routing_handler_file = $routing_handlers_path .'/'. $routing_handler_filename .".inc";
    
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
    
    //      debug
    if ( $enable_debug && $debug[ 'req' ]) {
        krumo( $request_arr );
    }
    
    
    
    //----------------------------------------------------------------------------------------------------------------
    //      Routing
    //      Include context-specific process code (corresponding to current URL)
    
    
    $context_handlers_arr = $request_arr;
    
    //      modif 2013/03/21 18:21:29 - in descending order (top > bottom, or container > contained)
    $prev_ch = '';
    
    foreach( $context_handlers_arr as $i => $ch )
    {
        if ( !empty( $prev_ch )) {
            $routing_handler_file = $prev_ch ."/". $ch .".inc";
        }
        else {
            $routing_handler_file = $routing_handlers_path .'/'. $ch .".inc";
        }
        
        $prev_ch = str_replace( '.inc', '', $routing_handler_file );
        
        if ( file_exists( $routing_handler_file ))
        {
            include $routing_handler_file;
            $is_404 = false;
            
            //      debug
            if ( !empty( $debug[ 'routing' ]))
            {
                print '<pre>';
                print "$routing_handler_file loaded. <--------- ok";
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
        $prev_ch = '';
        $stemmed_context_handlers_arr = $request_arr;
        array_shift( $stemmed_context_handlers_arr );
        
        foreach( $stemmed_context_handlers_arr as $i => $ch )
        {
            if ( !empty( $prev_ch )) {
                $routing_handler_file = $prev_ch ."_". $ch .".inc";
            }
            else {
                $routing_handler_file = $routing_handlers_path .'/'. $ch .".inc";
            }
            
            $prev_ch = str_replace( '.inc', '', $routing_handler_file );
            
            if ( file_exists( $routing_handler_file ))
            {
                include $routing_handler_file;
                $is_404 = false;
                
                //      debug
                if ( !empty( $debug[ 'routing' ]))
                {
                    print '<pre>';
                    print "$routing_handler_file loaded. <--------- ok";
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
    
    
    
    //----------------------------------------------------------------------------------------------------------------
    //      Templates
    //      Look for templates corresponding to current URL
    //      (the actual inclusion is done inside theme_rendering.inc)
    //      @see bootstrap.inc
    //      @see theme_rendering.inc
    
    
    $tpl_suggestions_arr = $request_arr;
    $tpl_suggestion = $theme_path .'/'. join( "/", $tpl_suggestions_arr );
    $most_specific_tpl_suggestion_matched = false;
    
    while ( !empty( $tpl_suggestions_arr ))
    {
        //      update 2014/07/27 02:26:38 - support other extensions usable as content tpl
        foreach( $content_tpl_extensions_arr as $content_tpl_extension )
        {
            if ( file_exists( $tpl_suggestion . $content_tpl_extension ))
            {
                $content_tpl = $tpl_suggestion . $content_tpl_extension;
                $is_404 = false;
                
                //      debug
                if ( !empty( $debug[ 'routing' ]))
                {
                    print '<pre>';
                    print "$tpl_suggestion$content_tpl_extension CHECKS <--------- ok";
                    print '</pre>';
                }
                
                //      hack 2013/03/25 15:25:39 - avoid 2nd pass when most specific match is reached
                if ( count( $tpl_suggestions_arr ) > 1 ) {
                    $most_specific_tpl_suggestion_matched = true;
                }
                
                break;
            }
            
            //      debug
            if ( !empty( $debug[ 'routing' ]))
            {
                print '<pre>';
                print "($tpl_suggestion$content_tpl_extension not found)";
                print '</pre>';
            }
        }
        
        array_pop( $tpl_suggestions_arr );
        $tpl_suggestion = $theme_path .'/'. join( "/", $tpl_suggestions_arr );
    }
    
    
    //      Special cases (rest / iframe)
    $run_2nd_pass = false;
    
    switch ( $request_arr[ 0 ])
    {
        case 'rest':
            $content_tpl_type = 'rest';
            if ( !$most_specific_tpl_suggestion_matched ) {
                $run_2nd_pass = true;
            }
            array_shift( $request_arr );
        break;
        
        case 'iframe':
            $content_tpl_type = 'iframe';
            if ( !$most_specific_tpl_suggestion_matched ) {
                $run_2nd_pass = true;
            }
            array_shift( $request_arr );
        break;
    }
    
    //      Load inside each possible "wrapper" template file, the "normal" candidate, lowest-specificity support (defaults)
    if ( $run_2nd_pass )
    {
        //      2nd pass for Theming : content template file to use
        $tpl_suggestions_arr = $request_arr;
        $tpl_suggestion = $theme_path .'/'. join( "/", $tpl_suggestions_arr );
        while ( !empty( $tpl_suggestions_arr ))
        {
            //      update 2014/07/27 02:26:38 - support other extensions usable as content tpl
            foreach( $content_tpl_extensions_arr as $content_tpl_extension )
            {
                if ( file_exists( $tpl_suggestion . $content_tpl_extension ))
                {
                    $content_tpl = $tpl_suggestion . $content_tpl_extension;
                    $is_404 = false;
                    
                    //      debug
                    if ( !empty( $debug[ 'routing' ]))
                    {
                        print '<pre>';
                        print "$tpl_suggestion$content_tpl_extension CHECKS <--------- ok";
                        print '</pre>';
                    }
                    
                    break;
                }
            
                //      debug
                if ( !empty( $debug[ 'routing' ]))
                {
                    print '<pre>';
                    print "($tpl_suggestion$content_tpl_extension not found)";
                    print '</pre>';
                }
            }
            
            array_pop( $tpl_suggestions_arr );
            $tpl_suggestion = $theme_path .'/'. join( "/", $tpl_suggestions_arr );
        }
        
        //      During 2nd pass, provide default template (in case none exists to match current path)
        if ( $tpl_suggestion == $theme_path .'/.tpl.php' && file_exists( $theme_path .'/raw.tpl.php' )) {
            $content_tpl = $theme_path .'/raw.tpl.php';
        }
    }
    
    //      404 handler
    if ( $is_404 ) {
        send_404();
    }
    
    //      modif 2013/03/21 15:14:42 - si on arrive jusqu'ici alors que l'URL n'est pas celle de l'index, on prend le default s'il existe
    elseif ( $content_tpl == $theme_path .'/index.tpl.php' && file_exists( $theme_path .'/default.tpl.php' )) {
        $content_tpl = $theme_path .'/default.tpl.php';
    }
}


