<?php

/**
 *  @file
 *  Server - edit
 *  @path /sytem/server/1/edit
 */

//      Init
$head_page_title = "Edit Server $server_title";

//      debug
//$content .= '';
//$content .= '<p>';
//$content .= "title = $server_title";
//$content .= '</p>';
//$content .= '<p>';
//$content .= "arg3 = $arg3";
//$content .= '</p>';

//      Loading data
$server_data = db_fetch( "SELECT * FROM `server` WHERE `id_server`=$arg2" );
if ( empty( $server_data ))
    $content .= alert( "Server not found.", 'error' );
else
{
    $head_page_title = "Manage ". $server_data[ 'title' ];
    
    $server_unserialized_data = array();
    if ( empty( $server_data[ 'data' ]))
        $content .= alert( "Server data seems incomplete.", 'warning' );
    else
        $server_unserialized_data = safe_unserialize( $server_data[ 'data' ]);
    
    //      Mapping
    $form_values = array(
        'id_server' => $server_data[ 'id_server' ],
        'title' => $server_data[ 'title' ],
        'description' => $server_data[ 'description' ],
        'host-name' => $server_data[ 'hostname' ],
        'ssh-user' => $server_unserialized_data[ 'ssh' ][ 'u' ],
        'password' => $server_unserialized_data[ 'ssh' ][ 'p' ],
    );
    
    //      Render form with its values
    $content .= render_form(
        
        //      This produces the rendered form HTML, with loaded values
        //      @evol 2013/07/16 00:42:23 - better CRUD abstraction...
        form_set_values( _server_form_content(), $form_values ),
        
        //      Submit handler
        '_server_form_submit',
        
        //      Form various options
        array(
            'validation_handler' => '_server_form_validate',
            
            //      Needed to avoid duplicates when changing hostname
            //      (because we reuse the 'creation' submit handler)
            'submit_handler_params' => array( 'id_server' => $server_data[ 'id_server' ]),
            
            'form_attributes' => array(
                'actions' => input( array(
                        'type' => 'submit',
                        'value' => 'Edit',
                        'class' => 'btn btn-primary',
                    )) .
                    " ".
                    l( "Delete", get_current_path() . "/delete", array( 'attributes' => array( 'class' => 'btn btn-danger' ))),
            ),
        )
    );
}


