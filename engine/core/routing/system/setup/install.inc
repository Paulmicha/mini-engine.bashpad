<?php

/**
 *  @file
 *  DB installation
 *  
 *  @path /system/setup/install
 */

//      Confirmation
if ( empty( $_GET[ 'yes' ]) && empty( $_POST[ 'poormans-submit-flag' ]))
{
    $head_page_title = "<i class='icon-download-alt'></i> Installation";
    
    //      Using entities
    //      Select '.inc' files to load configuration from
    $content .= "<p>Entities may define fields for the storage driver (these dictate SQL Tables and fields to be created).</p>";
    $content .= render_form( file_selector( array( 'engine/core/entities', 'config/entities' ), array(
        'mask' => '`.\.inc`i',
        'checked' => 'checked',
        'title' => "<i class='icon-long-arrow-right icon-large'></i> These entities must be installed :<br><small style='color:black'>Necessary SQL Tables will be created (if they don't exist yet)</small>",
    )));
}

//      Execution
else
{
    foreach( $_POST[ 'files' ] as $serialized_file_data )
    {
        $f = safe_unserialize( $serialized_file_data );
        $entity_name = str_replace( '.inc', '', $f->filename );
        
        //      Feedback
        $content .= "<hr>";
        $content .= "<p>Processing <strong>$entity_name</strong> - <code>". $f->fullpath ."</code> ...</p>";
        
        include $f->fullpath;
        
        $schema_sql = entity_sql_storage( $entity_name, $entity );
        
        //      debug
        //krumo( $schema_sql );
        //$content .= '<pre>';
        //$content .= $schema_sql;
        //$content .= '</pre>';
        
        //      Execute
        if ( !empty( $schema_sql ))
        {
            if ( !db_query( $schema_sql )) {
                $content .= alert( "<strong>Error</strong> : failed query ~ <code>". check_plain( print_r( $schema_sql, 1 )) ."</code>", 'error' );
            }
            else {
                $content .= alert( "ok", 'success' );
            }
        }
        else {
            $content .= alert( "This entity metadata did not allow to build a valid query.", 'warning' );
        }
        
        //      Debug
        if ( !empty( $debug[ 'installation' ]))
        {
            $content .= '<pre>';
            $content .= $schema_sql;
            $content .= '</pre>';
        }
    }
}


