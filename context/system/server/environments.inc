<?php

/**
 *  @file
 *  Server - home : list environments
 *  @path /sytem/server/1
 */

//      Init
$head_page_title = "Server $server_title";


//------------------------------------------------------------------------------
//      List environments

//      Results
$query = "SELECT * FROM `environment` WHERE id_server = ? ORDER BY id_environment DESC";
$results = db_fetchall( $query, array( $arg2 ));

//      debug
//$content .= kpr( $results );

if ( empty( $results ))
    $content .= alert( "There are no environments for this server.<br><i class='icon-arrow-right'></i> you should ". l( "Add a new one <i class='icon-plus'></i>", 'system/server/'. $arg2 .'/addenvironment' ));
else
{
    $lines = array();
    foreach( $results as $r )
    {
        if ( !empty( $r[ 'data' ]))
            $data = safe_unserialize( $r[ 'data' ]);
        
        //      Tags
        $tags = get_tags( 'environment', $r[ 'id_environment' ]);
        
        $lines[] = array(
            $r[ 'id_environment' ],
            l( $r[ 'title' ], 'system/server/'. $arg2 .'/env/'. $r[ 'id_environment' ]),
            $r[ 'description' ],
            $r[ 'namespace' ],
            view_tags( $tags ),
            db_popfetch( "SELECT COUNT(*) FROM `folder` WHERE `id_environment` = ?", array( $r[ 'id_environment' ])),
            l( "<i class='icon-pencil'></i>&nbsp;Edit", 'system/server/'. $arg2 .'/env/'. $r[ 'id_environment' ] .'/edit' ),
        );
    }
    $content .= table( $lines, array( 'header' => array(
        "ID",
        "Title",
        "Description",
        "Namespace <small>(auto match)</small>",
        "Tags",
        "Folders",
        "Actions"
    )));
}


