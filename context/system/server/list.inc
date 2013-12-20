<?php

/**
 *  @file
 *  Servers - list all
 *  @path /sytem/server
 */

//      Init
$head_page_title = "Listing all servers";
//$content .= view_db_table( 'server' );

//      Build list
$max = 500;
$results = db_fetchall( "SELECT * FROM `server` ORDER BY `id_server` DESC LIMIT 0,$max" );
if ( empty( $results ))
    $content .= alert( "There are no servers <i class='icon-arrow-right'></i> you should ". l( "Add a new one <i class='icon-plus'></i>", 'system/server/add' ));
else
{
    $lines = array();
    foreach( $results as $r )
    {
        $data = safe_unserialize( $r[ 'data' ]);
        $lines[] = array(
            $r[ 'id_server' ],
            l( $r[ 'title' ], 'system/server/'. $r[ 'id_server' ]),
            $r[ 'description' ],
            db_popfetch( "SELECT COUNT(*) FROM `environment` WHERE `id_server` = ?", array( $r[ 'id_server' ])),
            $r[ 'hostname' ],
            $data[ 'ssh' ][ 'u' ],
            l( "<i class='icon-pencil'></i>&nbsp;Edit", 'system/server/'. $r[ 'id_server' ] .'/edit' ) .
                '<br>' .
                l( "<i class='icon-plus'></i>&nbsp;Add environment", 'system/server/'. $r[ 'id_server' ] .'/addenvironment' ),
        );
    }
    $content .= table( $lines, array( 'header' => array(
        "ID",
        "Title",
        "Description",
        "Environments",
        "Hostname",
        "Username (SSH)",
        "Actions"
    )));
}


