<?php

/**
 *  @file
 *  Batch - View archived logs
 *  
 *  @path /batch/archives/123
 */

//      Load
$r = db_fetch( "SELECT * FROM `thread` WHERE `id_thread` = ?", array( $arg2 ));
$batch_title = db_popfetch( "SELECT title FROM batch WHERE id_batch = ?", array( $r[ 'id_batch' ]));
if ( !empty( $r[ 'data' ]))
    $r[ 'data' ] = safe_unserialize( $r[ 'data' ]);

//      debug
//krumo( $r );

$dblog_entries = get_batch_log( $arg2 );
$preprocessed_log_entries_arr = array();

//      debug
//krumo( $dblog_entries );

foreach( $dblog_entries as $step => $log )
{
    $env_data = db_fetch( "SELECT * FROM `environment` WHERE `id_environment` = ?", array( $log[ 'id_environment' ]));
    $server_data = db_fetch( "SELECT * FROM `server` WHERE `id_server` = ?", array( $env_data[ 'id_server' ]));
    
    $preprocessed_log_entries_arr[] = '<hr><p>Context :<br>Environment : '.
        '<strong>'.
        $env_data[ 'title' ] .
        '</strong>'.
        ' - Server : '.
        '<strong>'.
        $server_data[ 'title' ] .
        '</strong>'.
        ' <code>'.
        $server_data[ 'hostname' ] .
        '</code> '.
        '<br>
        Raw command <span class="muted">(as it was actually executed)</span> :'.
        '</p>'.
        view_source( $log[ 'cmd' ], 'bash' ) .
        '<p>'.
        "<strong>'Std out'</strong> feedback :".
        '</p>'.
        view_source( $log[ 'std_out' ], 'bash' ) .
        '';
}

//      Init
$head_page_title = "<i class='icon-hdd'></i> Batch '$batch_title' - Thread $arg2 logs";

//      batch data
$tags_batch = get_tags( 'batch', $r[ 'id_batch' ]);

//      List data
$content .= dl( array(
    "Preset" => $r[ 'preset' ],
    "State" => $r[ 'state' ],
    "Execution datestamp" => datestamp_format( $r[ 'datestamp' ]),
    "Batch Tags" => view_tags( $tags_batch ),
), array( 'class' => 'dl-vertical' ));

//      Logs
$content .= "<h2>Logs by steps</h2>".
    "<ol><li>". join( '</li><li>', $preprocessed_log_entries_arr ) ."</li></ol>";


