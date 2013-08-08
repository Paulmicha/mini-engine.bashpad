<?php

/**
 *  @file
 *  Commands - list all
 *  
 *  @path /sytem/command
 */

//      Init
$head_page_title = "<i class='icon-cogs'></i> Listing all commands";

//      Paged table view
$content .= render_pager_query_table( "SELECT * FROM `command` ORDER BY `id_command` DESC", array(), array(
    'preprocess_results' => '_preprocess_command',
    'table_options' => array(
        'header' => array(
            "ID",
            "Title",
            "Description",
            "Tags",
            "Configurable Tokens",
            "Actions",
        ),
    ),
));



/**
 *  Preprocess pager query results
 *  -> make table lines from query results
 */
function _preprocess_command( $results )
{
    $lines_arr = array();
    
    foreach( $results as $r )
    {
        $data = array();
        if ( !empty( $r[ 'data' ]))
            $data = safe_unserialize( $r[ 'data' ]);
        
        //      Tags
        $tags = get_tags( 'command', $r[ 'id_command' ]);
        
        //      Tokens
        $configurable_tokens_arr = array();
        if ( !empty( $data[ 'tokens' ]))
            foreach( $data[ 'tokens' ] as $configurable_token )
                $configurable_tokens_arr[] = '['. $configurable_token[ 'namespace' ] .']';
        
        $lines_arr[] = array(
            $r[ 'id_command' ],
            l( $r[ 'title' ], 'system/command/'. $r[ 'id_command' ]),
            $r[ 'description' ],
            view_tags( $tags ),
            join( ', ', $configurable_tokens_arr ),
            l( "<i class='icon-pencil'></i>&nbsp;Edit", 'system/command/'. $r[ 'id_command' ] .'/edit' ),
        );
    }
    
    return $lines_arr;
}


