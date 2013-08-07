<?php

/**
 *  @file
 *  Server - env
 *  
 *  @path /sytem/server/123/env/1
 *  @path /sytem/server/123/env/1/edit
 *  @path /sytem/server/123/env/1/delete
 */

//      Init
$head_page_title = "Manage environment $env_title in server $server_title";

//      Environment data
$tags_env = get_tags( 'environment', $arg4 );
$environment_data = db_fetch( "SELECT * FROM `environment` WHERE `id_environment` = ?", array( $arg4 ));
if ( !empty( $environment_data[ 'data' ]))
    $environment_data[ 'data' ] = safe_unserialize( $environment_data[ 'data' ]);

//      List Folders
$folders = db_fetchall( "SELECT * FROM `folder` WHERE `id_environment` = ? ORDER BY `id_folder` DESC", array( $arg4 ));



//-----------------------------------------------------------------------------------------------
//      Context : View


if ( empty( $arg5 ))
{
    $folder_html = '';
    $lines = array();
    if ( !empty( $folders ))
    {
        foreach( $folders as $f )
        {
            $tags = get_tags( 'folder', $f[ 'id_folder' ]);
            
            $lines[] = array(
                $f[ 'id_folder' ],
                $f[ 'path' ],
                view_tags( $tags ),
                $f[ 'owner_group' ],
                $f[ 'permissions' ],
            );
        }
        $folder_html .= table( $lines, array( 'header' => array(
            "ID",
            "Path",
            "Tags",
            "Owner : Group",
            "Permissions",
        )));
    }
    
    //      List data
    $env_data_arr = array(
        "Namespace" => $environment_data[ 'namespace' ],
        "URL" => l( $environment_data[ 'url' ], $environment_data[ 'url' ], array( 'external' => true )),
        //"id_server" => $environment_data[ 'id_server' ],
        "Tags" => view_tags( $tags_env ),
        "description" => $environment_data[ 'description' ],
    );
    $environment_html .= dl( $env_data_arr );
    
    //      Credentials
    if ( !empty( $environment_data[ 'data' ][ 'credentials' ]))
        foreach( $environment_data[ 'data' ][ 'credentials' ] as $credentials )
        {
            $type_prefix = ucfirst( $credentials[ 'type' ]);
            $env_credentials_dl_arr = array();
            
            if ( !empty( $credentials[ 'db_name' ]))
                $env_credentials_dl_arr[ "$type_prefix DB Name" ] = $credentials[ 'db_name' ];
            if ( !empty( $credentials[ 'username' ]))
                $env_credentials_dl_arr[ "$type_prefix Username" ] = $credentials[ 'username' ];
            if ( !empty( $credentials[ 'port' ]))
                $env_credentials_dl_arr[ "$type_prefix Port" ] = $credentials[ 'port' ];
            
            $environment_html .= '<hr>';
            $environment_html .= dl( $env_credentials_dl_arr );
        }
    
    //      View in 2 columns
    $content .= grid( array(
        array( "<h2>$env_title</h2>", "<h2>Folders</h2>" ),
        array( $environment_html, $folder_html ),
    ));
}



//-----------------------------------------------------------------------------------------------
//      Context : Edit


elseif ( $arg5 == 'edit' )
{
    $content .= "<h2>Edit environment $env_title</h2>";
    
    //      Multigroup : Credentials mapping preparation
    $credentials_mapping = array();
    if ( !empty( $environment_data[ 'data' ][ 'credentials' ]))
        foreach( $environment_data[ 'data' ][ 'credentials' ] as $i => $c )
        {
            $credentials_mapping[ 'multifield-credentials-type-of-credentials' ][ 'multigroup-'. $i ] = $c[ 'type' ];
            $credentials_mapping[ 'multifield-credentials-db-name' ][ 'multigroup-'. $i ] = $c[ 'db_name' ];
            $credentials_mapping[ 'multifield-credentials-port' ][ 'multigroup-'. $i ] = $c[ 'port' ];
            $credentials_mapping[ 'multifield-credentials-username' ][ 'multigroup-'. $i ] = $c[ 'username' ];
            $credentials_mapping[ 'multifield-credentials-password' ][ 'multigroup-'. $i ] = $c[ 'password' ];
        }
    
    //      Multigroup : Folders mapping preparation
    $folders_mapping = array();
    if ( !empty( $folders ))
        foreach( $folders as $i => $f )
        {
            //      Tags
            $tags = get_tags( 'folder', $f[ 'id_folder' ]);
            if ( !empty( $tags ))
                foreach( $tags as $id_tag )
                    $folders_mapping[ 'multifield-folders-folder-tag-s-' ][ 'multigroup-'. $i ][] = "id-$id_tag";
            
            //      Normal fields
            $folders_mapping[ 'multifield-folders-path' ][ 'multigroup-'. $i ] = $f[ 'path' ];
            $folders_mapping[ 'multifield-folders-owner-group' ][ 'multigroup-'. $i ] = $f[ 'owner_group' ];
            $folders_mapping[ 'multifield-folders-permissions' ][ 'multigroup-'. $i ] = $f[ 'permissions' ];
        }
    
    //      Multiselect : Tag(s) mapping preparation
    $tags_env_arr = array();
    foreach( $tags_env as $id_tag )
        $tags_env_arr[] = "id-$id_tag";
    
    //      Mapping loaded values to form inputs ('name' attributes)
    $form_values = array(
        'namespace' => $environment_data[ 'namespace' ],
        'title' => $environment_data[ 'title' ],
        'url' => $environment_data[ 'url' ],
        'description' => $environment_data[ 'description' ],
        'data' => $environment_data[ 'data' ],
        'tag-s-' => $tags_env_arr,
        'multigroups' => array(
            'credentials' => $credentials_mapping,
            'folders' => $folders_mapping,
        ),
    );
    
    //      debug
    //krumo( $form_values );
    
    //      Render form with its values
    $content .= render_form(
        form_set_values( _environment_form_content(), $form_values ),
        '_environment_form_submit',
        array(
            'validation_handler' => '_environment_form_validate',
            'submit_handler_params' => array( 'id_environment' => $environment_data[ 'id_environment' ]),
        )
    );
}



//-----------------------------------------------------------------------------------------------
//      Context : Delete


elseif ( $arg5 == 'delete' )
{
    $content .= "<h2>Delete environment $env_title</h2>";
    
    //      Confirmation
    if ( empty( $_GET[ 'yes' ]))
    {
        $content .= "<p>Really delete ?</p>";
        $content .= "<p>Note : all the folders defined for this environment will also be deleted from the database.</p>";
        $content .= "<p>";
        $content .= l( "<i class='icon-trash'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-danger' )));
        $content .= "</p>";
    }
    
    //      Deletion
    else if ( $_GET[ 'yes' ] == 'do-it' )
    {
        //      Delete environment
        $db_delete = db_delete( 'environment', array( 'id' => $arg4 ));
        if ( !empty( $db_delete ))
            $content .= alert( "Deletion successful", 'success' );
        else
            $content .= alert( "Deletion failed. That's all I know.", 'warning' );
        
        //      Delete folders
        $db_delete = db_delete( 'folder', array( 'where' => "`id_environment`=?", 'where_params' => array( $arg4 )));
        if ( !empty( $db_delete ))
            $content .= alert( "Folders deletion successful", 'success' );
        else
            $content .= alert( "Folders deletion failed. That's all I know.", 'warning' );
        
        //      Delete oprhaned tags relations
        //      @evol 2013/07/22 17:31:05
    }
}


