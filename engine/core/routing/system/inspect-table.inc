<?php

/**
 *  @file
 *  Inspect any DB Table
 *  @path /system/inspect-table
 */

//      Init
$head_page_title = "Inspect any DB Table";
$table = check_plain( arg( 2 ));

//      View selected table
if ( !empty( $table ))
    $content .= view_db_table( $table );
else
{
    $content .= "You may also use URL to enter the name of the table - ex: <a href='". get_current_url() ."/cache'>system/inspect-table/cache</code></a>";
    $content .= "<hr>";
    $content .= render_form( input_text( "Table" ), 'inspect_table_submit_handler' );
}


/**
 *  Form Submit Handler
 */
function inspect_table_submit_handler( $params_arr )
{
    $table = check_plain( $_POST[ 'table' ]);
    if ( !empty( $table ))
        return view_db_table( $table ); 
}


