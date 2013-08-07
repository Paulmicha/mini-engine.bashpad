<?php

/**
 *  @file
 *  System > Server dispatcher
 *  
 *  @path system/server
 *  @path system/server/<operation>
 *  @path system/server/123
 *  @path system/server/123/<operation>
 *  @path system/server/123/env
 *  @path system/server/123/env/123
 *  @path system/server/123/env/123/<operation>
 */

//      Init
$head_page_title = "Manage Servers";



//-------------------------------------------------------------------------------------------
//      Local menus


$arg2 = check_plain( arg( 2 ));
$arg3 = check_plain( arg( 3 ));
$arg4 = check_plain( arg( 4 ));
$arg5 = check_plain( arg( 5 ));

//      Menu tabs
$tab_items = array(
    array( "Servers list", 'system/server' ),
    array( "Add new server", 'system/server/add' ),
);
$subtab_items = array();
$subtab_tabs = array();
if ( is_numeric( $arg2 ))
{
    $server_title = db_popfetch( "SELECT `title` FROM `server` WHERE `id_server` = ?", array( $arg2 ));
    $tab_items[] = array( "Manage $server_title", 'system/server/'. $arg2, 'path_is_in_base_route' );
    
    if ( is_numeric( $arg4 ))
        $subtab_items[] = array( "Environments", 'system/server/'. $arg2, 'path_is_in_base_route' );
    else
        $subtab_items[] = array( "Environments", 'system/server/'. $arg2 );
    
    $subtab_items[] = array( "<i class='icon-pencil'></i>&nbsp;Edit", 'system/server/'. $arg2 .'/edit' );
    $subtab_items[] = array( "<i class='icon-dashboard'></i>&nbsp;Check status", 'system/server/'. $arg2 .'/status' );
    $subtab_items[] = array( "<i class='icon-trash'></i> Delete", 'system/server/'. $arg2 .'/delete' );
    $subtab_items[] = array( "<i class='icon-plus'></i> Add Environment", 'system/server/'. $arg2 .'/addenvironment' );
    
    if ( is_numeric( $arg4 ))
    {
        $env_title = db_popfetch( "SELECT `title` FROM `environment` WHERE `id_environment` = ?", array( $arg4 ));
        $subtab_tabs[] = array( "$env_title", 'system/server/'. $arg2 .'/env/'. $arg4 );
        $subtab_tabs[] = array( "<i class='icon-pencil'></i> Edit $env_title", 'system/server/'. $arg2 .'/env/'. $arg4 .'/edit' );
        $subtab_tabs[] = array( "<i class='icon-trash'></i> Delete $env_title", 'system/server/'. $arg2 .'/env/'. $arg4 .'/delete' );
    }
}
$content .= nav_tabs( $tab_items );
if ( !empty( $subtab_items ))
    $content .= nav_pills( $subtab_items );
if ( !empty( $subtab_tabs ))
    $content .= nav_tabs( $subtab_tabs );



//-------------------------------------------------------------------------------------------
//      Dispatch


//      Default handler
if ( empty( $arg2 ))
    include "context/system/server/list.inc";

//      Including corresponding files from paths
else if ( is_numeric( $arg2 ))
    if ( empty( $arg3 ))
        include "context/system/server/environments.inc";
    else
        include "context/system/server/$arg3.inc";
else
    include "context/system/server/$arg2.inc";



//-------------------------------------------------------------------------------------------
//      Form CRUD functions


/**
 *  Server edit form
 */
function _server_form_content()
{
    $form_html = '';
    $form_html .= input_text( 'Title' );
    $form_html .= textarea( 'Description' );
    $form_html .= "<hr>";
    $form_html .= input_text( 'Host Name', array( 'input_attributes' => array( 'help-inline' =>  "Or, IP address" )));
    $form_html .= input_text( 'SSH user' );
    $form_html .= input_password( 'Password' );
    
    return $form_html;
}



/**
 *  Server validation
 */
function _server_form_validate( &$submit_handler_params )
{
    $return = array();
    
    //      Quick sanitization (no multi-value in this form)
    $posted = $_POST;
    form_recursive_sanitization( $posted );
    
    //      Save sanitized values for render_form()
    $return[ 'sanitized_values' ] = $posted;
    
    //      Error checking
    if ( !empty( $posted[ 'host-name' ]) && !empty( $posted[ 'title' ]))
    {
        //      Test if existing
        //      Test if we must do the test !
        if ( $submit_handler_params[ 'is_new' ])
        {
            $existing_test = db_fetch( 'SELECT * FROM server WHERE `hostname` LIKE ?', array( $posted[ 'host-name' ]));
            if ( !empty( $existing_test[ 'hostname' ]))
                $return[ 'errors' ] = alert( "It seems this <code>hostname</code> is already taken : ". $existing_test[ 'hostname' ], 'warning' );
        }
        
        //      Are we still good to proceed ?
        if ( empty( $return[ 'errors' ]))
        {
            //      Prepare DB record insertion
            $return[ 'values' ] = array(
                'title' => $posted[ 'title' ],
                'description' => $posted[ 'description' ],
                'hostname' => $posted[ 'host-name' ],
                'data' => safe_serialize( array(
                    'ssh' => array( 'u' => $posted[ 'ssh-user' ], 'p' => $posted[ 'password' ]),
                )),
            );
        }
    }
    else
        $return[ 'errors' ][] = "Missing 'title' or 'hostname'";
    
    return $return;
}



/**
 *  Submit handler
 */
function _server_form_submit( $params )
{
    //      debug
    //return kpr( $_POST );
    //krumo( $params );
    
    //      @see _server_form_validate()
    if ( !empty( $params[ 'values' ]))
    {
        //      Creation
        if ( $params[ 'is_new' ])
            $id = insert_if_no_existing_record( 'server', $params[ 'values' ], array(
                'unique_field_name' => 'hostname',
            ));
        
        //      Edition
        else if ( $params[ 'id_server' ])
            $id = db_update( 'server', $params[ 'values' ], array(
                'id' => $params[ 'id_server' ],
            ));
        
        //      Feedback
        if ( $id )
            return alert( "Server has been saved. <code>id = $id </code>", 'success' );
    }
    
    return alert( "Error : cannot save the server. That's all I know.", 'error' );
}



//-------------------------------------------------------------------------------------------
//      Form CRUD functions for environments


/**
 *  environment edit form
 */
function _environment_form_content()
{
    $form_html = '';
    $tags_select_options = tags_select_options();
    
    //      debug
    //if ( empty( $tags_select_options ))
    //    $tags_select_options = array( 'id-0' => "A tag" );
    
    //      Environment
    $form_html .= input_text( 'Namespace', array( 'input_attributes' => array( 'help-inline' => "Unique name across <strong>≠</strong> servers", 'required' => true )));
    $form_html .= input_text( 'Title', array( 'input_attributes' => array( 'required' => true )));
    $form_html .= textarea( 'Description' );
    $form_html .= input_text( 'url', array( 'input_attributes' => array( 'help-inline' => "of the project (if applicable)" )));
    
    //      Environment Tags
    if ( !empty( $tags_select_options ))
        $form_html .= multiple_select( 'Tag(s)', $tags_select_options );
    $form_html .= input_text( 'New Tag(s)', array( 'input_attributes' => array( 'help-inline' => "Multiple tags must be comma-separated <code>,</code>" )));
    
    //      DB Credentials
    $form_html .= "<hr>";
    $form_html .= multigroup( "Credentials", array(
        array(
            'func' => 'select',
            'args' => array( "Type of Credentials", array(
                '' => "---",
                //'user' => "Application user",
                'mysql' => "MySQL Database",
                'pgsql' => "PostgreSQL Database",
                //'api' => "API",
                //"Email",
                "htaccess" => ".htaccess (HTTP) authentication",
                'mongodb' => "MongoDB",
                'memcache' => "Memcache",
                'mariadb' => "MariaDB",
                'couchdb' => "CouchDB",
                'redis' => "Redis",
            )),
        ),
        //array(
        //    'func' => 'input_text',
        //    'args' => array( "Credentials Namespace", array( 'input_attributes' => array( 'help-inline' => "Complementary information - e.g. User account Role, API name..." ))),
        //),
        array(
            'func' => 'input_text',
            'args' => array( "DB Name", array( 'input_attributes' => array( 'help-inline' => "(if applicable)" ))),
        ),
        array(
            'func' => 'input_text',
            'args' => array( "Port", array( 'append' => 'integer', 'input_attributes' => array( 'help-inline' => "(if applicable)" ))),
        ),
        //array(
        //    'func' => 'input_text',
        //    'args' => array( "DB Host", array( 'input_attributes' => array( 'help-inline' => "(if applicable)" ))),
        //),
        array(
            'func' => 'input_text',
            'args' => array( "Username" ),
        ),
        array(
            'func' => 'input_password',
            'args' => array( "Password" ),
        ),
    ));
    
    //      Folder
    $form_html .= "<hr>";
    $folder_multigroup_fields = array(
        array(
            'func' => 'input_text',
            'args' => array( "Path" ),
        ),
        array(
            'func' => 'input_text',
            'args' => array( "Owner:Group", array( 'input_attributes' => array( 'help-inline' => "for <code>chown</code> - leave blank if unsure." ))),
        ),
        array(
            'func' => 'input_text',
            'args' => array( "Permissions", array( 'input_attributes' => array( 'help-inline' => "for <code>chmod</code> - leave blank if unsure." ))),
        ),
    );
    
    //      Folder tags
    if ( !empty( $tags_select_options ))
        $folder_multigroup_fields[] = array(
            'func' => 'select',
            'args' => array( "Folder Tag(s)", $tags_select_options, array( 'select_attributes' => array( 'multiple' => 'multiple', 'help-inline' => "Tags" ))),
        );
    $folder_multigroup_fields[] = array(
        'func' => 'input_text',
        'args' => array( "New Folder Tag(s)", array( 'input_attributes' => array( 'help-inline' => "Multiple tags must be comma-separated <code>,</code>" ))),
    );
    
    $form_html .= multigroup( "Folders", $folder_multigroup_fields );
    
    return $form_html;
}



/**
 *  environment validation
 */
function _environment_form_validate( &$submit_handler_params )
{
    $return = array();
    
    //      debug
    //krumo( $_POST );
    
    //      Quick sanitization
    $posted = $_POST;
    form_recursive_sanitization( $posted );
    
    //      Save sanitized values for render_form()
    $return[ 'sanitized_values' ] = $posted;
    
    //      Error checking
    if ( !empty( $posted[ 'namespace' ]) && !empty( $posted[ 'title' ]))
    {
        //      Test if existing
        if ( $submit_handler_params[ 'is_new' ])
        {
            $existing_test = db_fetch( 'SELECT * FROM environment WHERE `namespace` LIKE ?', array( $posted[ 'namespace' ]));
            if ( !empty( $existing_test[ 'hostname' ]))
                $return[ 'errors' ] = alert( "It seems this <code>namespace</code> is already taken on that server : ". $existing_test[ 'hostname' ], 'warning' );
        }
        
        
        //      Are we still good to proceed ?
        if ( empty( $return[ 'errors' ]))
        {
            //      Prepare DB record insertion
            $submit_handler_params[ 'values' ] = array(
                'id_server' => intval( arg( 2 )),
                'namespace' => $posted[ 'namespace' ],
                'title' => $posted[ 'title' ],
                'url' => $posted[ 'url' ],
                'description' => $posted[ 'description' ],
            );
            
            //      Credentials : in serialized data
            $credentials_mapping = array(
                'multifield-credentials-type-of-credentials' => 'type',
                'multifield-credentials-db-name' => 'db_name',
                'multifield-credentials-port' => 'port',
                'multifield-credentials-username' => 'username',
                'multifield-credentials-password' => 'password',
            );
            foreach( $posted as $name => $multivalues )
                if ( substr( $name, 0, 22 ) == 'multifield-credentials' )
                    foreach( $multivalues as $group => $value )
                        if ( !empty( $value ))
                            $submit_handler_params[ 'values' ][ 'data' ][ 'credentials' ][ intval( str_replace( 'multigroup-', '', $group ))][ $credentials_mapping[ $name ]] = $value;
            
            //      Serialize data
            $submit_handler_params[ 'values' ][ 'data' ] = safe_serialize( $submit_handler_params[ 'values' ][ 'data' ]);
            
            
            //      Values for 'tag' DB table (for environment)
            //      Note the structure : anticipate helper function (the key '#0' is to be replaced by the entity id)
            //      @see _environment_form_submit()
            //      @see tags_set()
            $submit_handler_params[ 'tags' ][ 'environment' ][ '#0' ][ 'tags' ] = clean_values_tags_select_options( $posted[ 'tag-s-' ]);
            $submit_handler_params[ 'tags' ][ 'environment' ][ '#0' ][ 'new_tags' ] = explode( ',', $posted[ 'new-tag-s-' ]);
            
            //      Values for 'folder' DB table
            $submit_handler_params[ 'values_folders' ] = array();
            $folder_mapping = array(
                'multifield-folders-path' => 'path',
                'multifield-folders-owner-group' => 'owner_group',
                'multifield-folders-permissions' => 'permissions',
            );
            foreach( $posted as $name => $multivalues )
            {
                if ( substr( $name, 0, 18 ) == 'multifield-folders' )
                {
                    foreach( $multivalues as $group => $value )
                    {
                        $folder_num = intval( str_replace( 'multigroup-', '', $group ));
                        if ( !empty( $value ))
                        {
                            //      Values for 'tag' DB table (for folders)
                            if ( substr_count( $name, 'tag' ))
                            {
                                if ( $name == 'multifield-folders-folder-tag-s-' )
                                    $submit_handler_params[ 'tags' ][ 'folder' ][ '#'. $folder_num ][ 'tags' ] = clean_values_tags_select_options( $value );
                                
                                elseif ( $name == 'multifield-folders-new-folder-tag-s-' )
                                    $submit_handler_params[ 'tags' ][ 'folder' ][ '#'. $folder_num ][ 'new_tags' ] = array_append_values( $submit_handler_params[ 'tags' ][ 'new_tags' ][ 'folder' ][ $folder_num ], explode( ',', $value ));
                            }
                            
                            else
                                $submit_handler_params[ 'values_folders' ][ $folder_num ][ $folder_mapping[ $name ]] = $value;
                        }
                    }
                }
            }
        }
    }
    else
        $return[ 'errors' ][] = "Missing 'Title' or 'Namespace'";
    
    
    //      debug
    //$return[ 'errors' ][] = "debug";
    //krumo( $submit_handler_params );
    
    return $return;
}



/**
 *  Submit handler
 */
function _environment_form_submit( $params )
{
    $return = '';
    $error_flag = false;
    
    //      debug
    //krumo( $params );
    
    //      Environment
    if ( !empty( $params[ 'values' ]))
    {
        //      Creation
        if ( $params[ 'is_new' ])
            $id_environment = insert_if_no_existing_record( 'environment', $params[ 'values' ], array(
                'unique_field_name' => 'namespace',
            ));
        
        //      Edition
        else if ( $params[ 'id_environment' ])
            $id_environment = db_update( 'environment', $params[ 'values' ], array(
                'id' => $params[ 'id_environment' ],
            ));
        
        if ( $id_environment )
        {
            //      Feedback
            $return .= alert( "Environment '". $params[ 'values' ][ 'title' ] ."' has been saved <code>id_environment = $id_environment </code>", 'success' );
            
            //      Prepare Tags : replace the key for helper function
            $params[ 'tags' ][ 'environment' ][ $id_environment ] = $params[ 'tags' ][ 'environment' ][ '#0' ];
            unset( $params[ 'tags' ][ 'environment' ][ '#0' ]);
        }
        
        //      Feedback error
        else
        {
            $return .= alert( "Error : cannot save the environment '". $params[ 'values' ][ 'title' ] ."'. That's all I know.", 'error' );
            $error_flag = true;
        }
    }
    
    //      Folders
    if ( $id_environment )
    {
        //      Handle folder removal : need to handle 'orphans' when a folder is removed from the multigroup
        //      -> prepare diff
        $folders_before = db_fetchall( "SELECT * FROM `folder` WHERE `id_environment` = ? ORDER BY `id_folder` DESC", array( $id_environment ));
        $id_folder_saved_arr = array();
        
        if ( !empty( $params[ 'values_folders' ]))
        {
            foreach( $params[ 'values_folders' ] as $folder_num => $values )
            {
                $values[ 'id_environment' ] = $id_environment;
                
                //      debug
                //krumo( $values );
                
                $id_folder = insert_or_update( 'folder', $values, array(
                    'detect_duplicate_query' => "SELECT `id_folder` FROM `folder` WHERE `path` LIKE '". $values[ 'path' ] ."' AND `id_environment` = $id_environment",
                ));
                
                if ( $id_folder )
                {
                    //      Feedback
                    $return .= alert( "Folder '". $values[ 'path' ] ."' has been saved <code>id_folder = $id_folder </code>", 'success' );
                
                    //      Prepare Tags : replace the key for helper function
                    $params[ 'tags' ][ 'folder' ][ $id_folder ] = $params[ 'tags' ][ 'folder' ][ '#'. $folder_num ];
                    unset( $params[ 'tags' ][ 'folder' ][ '#'. $folder_num ]);
                    
                    //      For diff to check for oprhans
                    $id_folder_saved_arr[] = $id_folder;
                }
                
                //      Feedback error
                else
                {
                    $return .= alert( "Error : cannot save folder '". $values[ 'path' ] ."'. That's all I know.", 'error' );
                    $error_flag = true;
                }
            }
        }
        
        //      Orphan folders check by diff with previous folders
        //      (except in case of error)
        if ( !$error_flag && !empty( $folders_before ))
            foreach( $folders_before as $previous_folder )
                if ( !in_array( $previous_folder[ 'id_folder' ], $id_folder_saved_arr ))
                {
                    $success_flag = db_delete( 'folder', array( 'id' => $previous_folder[ 'id_folder' ]));
                    if ( $success_flag )
                        $return .= alert( "Folder '". $previous_folder[ 'path' ] ."' has been deleted <code>id_folder = ". $previous_folder[ 'id_folder' ] ." </code>", 'info' );
                    else
                        $return .= alert( "Error : cannot deleted folder '". $previous_folder[ 'path' ] ."' <code>id_folder = ". $previous_folder[ 'id_folder' ] ." </code>. That's all I know.", 'error' );
                }
    }
    
    //      debug
    //krumo( $params );
    
    //      Set Tags
    set_tags( $params[ 'tags' ]);
    
    return $return;
}


