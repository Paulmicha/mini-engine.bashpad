<?php

/**
 *  @file
 *  Batch - Presets
 *  @path /batch/presets
 */

//      Init
$head_page_title = "<i class='icon-road'></i> Batch Presets";

//      Description
$content .= "<p>This is where we may launch the same batch again (the ones saved as presets).</p>";

//      Paged table view
$content .= render_pager_query_table( "SELECT * FROM `thread` WHERE `preset` = 1 ORDER BY `id_thread` DESC", array(), array(
    'preprocess_results' => '_preprocess_archive',
    'table_options' => array(
        'header' => array(
            "ID",
            "Execute",
            "Title",
            "Description",
            "Batch",
            "Tags",
            "Steps",
            "Servers",
            "Environments",
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
        
        //      debug
        //krumo( $r[ 'data' ]);
        
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
        
        //      Special to presets : description
        //      @evol 2013/08/07 01:00:52
        //$description = '';
        //if ( !empty( $r[ 'data' ][ 'description' ]))
        //    $description = $r[ 'data' ][ 'description' ];
        
        //      Assemble columns
        $lines_arr[] = array(
            
            //      ID Thread
            $r[ 'id_thread' ],
            
            //      Launch
            l( "<i class='icon-rocket'></i>&nbsp;&nbsp;Launch", 'batch/presets/'. $r[ 'id_thread' ], array( 'attributes' => array(
                'class' => "btn btn-small btn-primary",
            ))),
            
            //      Title
            $r[ 'data' ][ 'preset-title' ],
            
            //      Description
            $r[ 'data' ][ 'preset-description' ],
            
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
        );
    }
    
    return $lines_arr;
}


