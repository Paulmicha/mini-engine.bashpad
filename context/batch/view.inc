<?php

/**
 *  @file
 *  Batch - view
 *  @path /sytem/batch/123
 */

//      Init
$head_page_title = "$batch_title summary";

//      batch data
$tags_batch = get_tags( 'batch', $arg1 );
$batch_data = db_fetch( "SELECT * FROM `batch` WHERE `id_batch` = ?", array( $arg1 ));

//      List data
$batch_data_list_arr = array(
    "Tags" => view_tags( $tags_batch ),
    "description" => $batch_data[ 'description' ],
);

//      Batch steps summary
$tokens_html = '';
$lines = array();
$batch_steps_arr = db_fetchall( "SELECT * FROM `batch_step` WHERE `id_batch` = ? ORDER BY `step` ASC", array( $arg1 ));
if ( !empty( $batch_steps_arr ))
{
    foreach( $batch_steps_arr as $step )
    {
        //      debug
        //krumo( $step );
        
        //      Prepare data
        $env = db_fetch( "SELECT * FROM `environment` WHERE `id_environment` = ?", array( $step[ 'id_environment' ]));
        $command = db_fetch( "SELECT * FROM `command` WHERE `id_command` = ?", array( $step[ 'id_command' ]));
        
        //      debug
        //krumo( $env );
        
        $lines[] = array(
            $step[ 'step' ],
            $step[ 'title' ],
            db_popfetch( "SELECT `title` FROM `server` WHERE `id_server` = ?", array( $env[ 'id_server' ])) ." : ". $env[ 'title' ] ." (". $env[ 'namespace' ] .")",
            "<strong>". $command[ 'title' ] ."</strong><br>". $command[ 'description' ],
        );
    }
    
    $tokens_html .= table( $lines, array( 'header' => array(
        "N",
        "Title",
        "Environment",
        "Command Description",
        //"Command",
    )));
}


//      View in 2 columns
$content .= grid( array(
    array( array( 'content' => "<h2>Details</h2>", 'attributes' => array( 'class' => "span3" )), array( 'content' => "<h2>Steps</h2>", 'attributes' => array( 'class' => "span9" ))),
    array( array( 'content' => dl( $batch_data_list_arr, array( 'class' => 'dl-vertical' )), 'attributes' => array( 'class' => "span3" )), array( 'content' => $tokens_html, 'attributes' => array( 'class' => "span9" ))),
));


