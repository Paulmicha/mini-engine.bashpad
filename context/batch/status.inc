<?php

/**
 *  @file
 *  batchs - add
 *  @path /sytem/batch/add
 */

//      Init
$head_page_title = "<i class='icon-dashboard'></i> Batch Processes status";

//      Description
$content .= "<p>This is where we can see if there's any currently running process, and their status.</p>";
$content .= "<p>You need to ". reload_link( "reload this page" ) ." from time to time in order to get an up-to-date status report.</p>";

//      Paged table view
$content .= render_pager_query_table( "SELECT * FROM `thread` WHERE `state` IN (?,?,?,?) ORDER BY `id_thread` DESC", array( 'init', 'standby', 'processing', 'done' ), array(
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
            "State",
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
        
        //      Prepare 'servers' and 'environments' column
        $servers_arr = array();
        $environments_arr = array();
        if ( !empty( $r[ 'data' ][ 'execution_steps_config' ]))
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
            
            //      State
            $r[ 'state' ],
        );
    }
    
    return $lines_arr;
}



