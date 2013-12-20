<?php

/**
 *  @file
 *  Server - delete
 *  @path /sytem/server/1/delete
 */

//      Init
$head_page_title = "Delete Server $server_title";

//      Confirmation
if ( empty( $_GET[ 'yes' ]))
{
    $content .= "<p>Really delete ?</p>";
    $content .= "<p>";
    $content .= l( "<i class='icon-trash'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-danger' )));
    $content .= "</p>";
}

//      Deletion
else if ( $_GET[ 'yes' ] == 'do-it' )
{
    $db_delete = db_delete( 'server', array( 'id' => $arg2 ));
    if ( !empty( $db_delete ))
        $content .= alert( "Deletion successful", 'success' );
    else
        $content .= alert( "Deletion failed. That's all I know.", 'warning' );
}


