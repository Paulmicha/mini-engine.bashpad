<?php

/**
 *  @file
 *  DB uninstallation - drop all tables
 *  
 *  @path /system/setup/uninstall
 */

$head_page_title = "<i class='icon-trash'></i> Uninstall = Drop DB tables";


//      Confirmation
if ( empty( $_GET[ 'yes' ]) && empty( $_POST[ 'poormans-submit-flag' ]))
{
    $content .= "<p>SQL Tables corresponding to the following entities will all be <strong>DROPPED</strong> (= permanently destroyed).</p>";
    
    //$content .= "<p>";
    //$content .= l( "<i class='icon-trash'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-danger' )));
    //$content .= "</p>";
    
    $content .= render_form(
        file_selector( array( 'engine/core/entities', 'config/entities' ), array(
            'mask' => '`.\.inc`i',
            'checked' => 'checked',
            'title' => "<i class='icon-long-arrow-right icon-large'></i> These entities will be uninstalled :",
        )),
        null,
        array(
            'form_attributes' => array(
                'actions' => '<button class="btn btn-large btn-danger"><i class="icon-trash"></i>&nbsp;Yes, do it</button>',
            ),
        )
    );
}

//      Execution
else
{
    //      debug
    //krumo( $_POST );
    
    foreach( $_POST[ 'files' ] as $serialized_file_data )
    {
        $f = safe_unserialize( $serialized_file_data );
        $entity_name = str_replace( '.inc', '', $f->filename );
        
        //      Feedback
        $content .= "<hr>";
        $content .= "<p>Processing <strong>$entity_name</strong> - <code>". $f->path .'/'. $f->filename ."</code> ...</p>";
        
        include $f->path .'/'. $f->filename;
        $destroy_sql = entity_sql_destroy( $entity_name, $entity );
        
        //      Execute
        if ( !empty( $destroy_sql ))
        {
            if ( !db_query( $destroy_sql )) {
                $content .= alert( "<strong>Error</strong> : failed query ~ <code>". check_plain( print_r( $destroy_sql, 1 )) ."</code>", 'error' );
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
            $content .= $destroy_sql;
            $content .= '</pre>';
        }
    }
}


