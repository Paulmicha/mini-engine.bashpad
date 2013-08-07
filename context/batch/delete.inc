<?php

/**
 *  @file
 *  Batch - delete
 *  @path /sytem/batch/123/delete
 */

//      Init
$head_page_title = "Delete Batch $batch_title";

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
    //      Deleting batch steps
    $db_delete = db_delete( 'batch_step', array(
        'where' => "`id_batch` = ?",
        'where_params' => array( $arg1 ),
    ));
    
    if ( !empty( $db_delete ))
    {
        $content .= alert( "All batch steps for id_batch <code>$arg1</code> have been deleted.", 'success' );
    
        //      Deleting batch
        $db_delete = db_delete( 'batch', array( 'id' => $arg1 ));
        if ( !empty( $db_delete ))
            $content .= alert( "Deletion successful", 'success' );
        else
            $content .= alert( "Deletion failed. That's all I know.", 'warning' );
    }
    else
        $content .= alert( "Deletion failed. That's all I know.", 'warning' );
}


