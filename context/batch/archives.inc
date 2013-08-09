<?php

/**
 *  @file
 *  Batch - Archives
 *  
 *  @path /batch/archives
 */

//      Init
$head_page_title = "<i class='icon-hdd'></i> Listing all batch operations previously launched";

//      Paged table view
$content .= render_pager_query_table( "SELECT * FROM `thread` ORDER BY `id_thread` DESC", array(), array(
    'preprocess_results' => '_preprocess_archive',
    'pager_query_options' => array(
        'nb_per_page' => 10,
    ),
    'table_options' => array(
        'header' => array(
            "ID Thread",
            "Time",
            "Batch",
            "Tags",
            "Steps",
            "Servers",
            "Environments",
            "Logs",
        ),
    ),
));



/**
 *  Preprocess pager query results
 *  -> make table lines from query results
 */
function _preprocess_archive( $results )
{
    $lines_arr = array();
    
    foreach( $results as $r )
    {
        $batch_title = db_popfetch( "SELECT title FROM batch WHERE id_batch = ?", array( $r[ 'id_batch' ]));
        
        if ( !empty( $r[ 'data' ]))
            $r[ 'data' ] = safe_unserialize( $r[ 'data' ]);
        
        //      Skip if thread has not been executed yet
        if ( empty( $r[ 'data' ][ 'execution_steps_config' ]))
            continue;
        
        //      Prepare 'servers' and 'environments' column
        $servers_arr = array();
        $environments_arr = array();
        foreach( $r[ 'data' ][ 'execution_steps_config' ] as $step )
        {
            $environments_arr[] = l( $step[ 'environment' ][ 'title' ], $step[ 'environment' ][ 'url' ], array( 'external' => true ));
            $servers_arr[] = $step[ 'environment' ][ 'server' ][ 'title' ] .
                " ".
                "<code>".
                $step[ 'environment' ][ 'server' ][ 'hostname' ] .
                "</code>";
        }
        
        //      Assemble columns
        $lines_arr[] = array(
            
            //      ID Thread
            $r[ 'id_thread' ],
            
            //      Time
            datestamp_format( $r[ 'datestamp' ]),
            
            //      Batch
            $batch_title,
            
            //      Tags
            view_tags( get_tags( 'batch', $r[ 'id_batch' ])),
            
            //      Steps
            count( $r[ 'data' ][ 'execution_steps_config' ]),
            
            //      Servers
            join( ', ', $servers_arr ),
            
            //      Environments
            join( ', ', $environments_arr ),
            
            //      Logs
            l( "Detailed logs", 'batch/archives/'. $r[ 'id_thread' ], array( 'external' => true )),
        );
    }
    
    return $lines_arr;
}


