<?php

/**
 *  @file
 *  Batch - setup (choose & setup)
 *  
 *  @path /batch/setup
 *  @path /batch/setup/123
 */


//      Init
$head_page_title = "<i class='icon-rocket'></i> Choose a batch to launch";

//      Paged table view
$content .= render_pager_query_table( "SELECT * FROM `batch` ORDER BY `id_batch` DESC", array(), array(
    'preprocess_results' => '_preprocess_batch',
    //'pager_query_options' => array(
    //    'nb_per_page' => 10,
    //),
    'table_options' => array(
        'header' => array(
            "ID",
            "Execute",
            "Title",
            "Description",
            "Steps",
            "Tags",
            "Actions",
        ),
    ),
));



/**
 *  Preprocess pager query results
 *  -> make table lines from query results
 */
function _preprocess_batch( $results )
{
    $lines_arr = array();
    
    foreach( $results as $r )
    {
        $data = array();
        if ( !empty( $r[ 'data' ]))
            $data = safe_unserialize( $r[ 'data' ]);
        
        //      Tags
        $tags = get_tags( 'batch', $r[ 'id_batch' ]);
        
        $lines_arr[] = array(
            $r[ 'id_batch' ],
            l( "<i class='icon-wrench'></i>&nbsp;Setup for launch", 'batch/'. $r[ 'id_batch' ] .'/render', array( 'attributes' => array( 'class' => 'btn btn-small btn-primary' ))),
            l( $r[ 'title' ], 'batch/'. $r[ 'id_batch' ]),
            $r[ 'description' ],
            db_popfetch( "SELECT COUNT(*) FROM `batch_step` WHERE `id_batch` = ?", array( $r[ 'id_batch' ])),
            view_tags( $tags ),
            l( "<i class='icon-pencil'></i>&nbsp;Edit", 'batch/'. $r[ 'id_batch' ] .'/edit' )
                ."  ".
                l( "<i class='icon-trash'></i>&nbsp;Delete", 'batch/'. $r[ 'id_batch' ] .'/delete' )
        );
    }
    
    return $lines_arr;
}


