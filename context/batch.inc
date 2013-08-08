<?php

/**
 *  @file
 *  Batch operations
 *  
 *  @path /batch
 *  @path /batch/<operation>
 *  @path /batch/<operation>/...
 */

//      Init
$head_page_title = "Batch operations";



//-------------------------------------------------------------------------------------------
//      Local menus


$arg1 = check_plain( arg( 1 ));
$arg2 = check_plain( arg( 2 ));
$arg3 = check_plain( arg( 3 ));

$tab_items = array(
    array( "Batch", 'batch' ),
    array( "Presets", 'batch/presets' ),
    array( "Archives", 'batch/archives' ),
    array( "Status", 'batch/status' ),
);
$subtab_items = array();
$subtab_tabs = array();

//      "Home" of Batch section : subtabs already active (add / setup existing)
if ( empty( $arg1 ) || $arg1 == 'add' )
{
    $tab_items[ 0 ] = array( "Batch", 'batch', 'path_is_in_base_route' );
    $subtab_items[] = array( "<i class='icon-rocket'></i>&nbsp;Select batch to launch", 'batch' );
    $subtab_items[] = array( "<i class='icon-plus'></i>&nbsp;Add new Batch", 'batch/add' );
}
elseif ( is_numeric( $arg1 ))
{
    $batch_title = db_popfetch( "SELECT `title` FROM `batch` WHERE `id_batch` = ?", array( $arg1 ));
    $tab_items[ 0 ] = array( "Batch", 'batch', 'path_is_in_base_route' );
    
    $subtab_items[] = array( $batch_title, 'batch/'. $arg1 );
    $subtab_items[] = array( "<i class='icon-pencil'></i>&nbsp;Edit", 'batch/'. $arg1 .'/edit' );
    $subtab_items[] = array( "<i class='icon-wrench'></i>&nbsp;Setup for launch", 'batch/'. $arg1 .'/render' );
    $subtab_items[] = array( "<i class='icon-trash'></i> Delete", 'batch/'. $arg1 .'/delete' );
}

$content .= nav_tabs( $tab_items );

if ( !empty( $subtab_items ))
    $content .= nav_pills( $subtab_items );



//-------------------------------------------------------------------------------------------
//      Dispatch


//      Default handler
if ( empty( $arg1 ))
    include "context/batch/list.inc";

//      Including corresponding files from paths
else if ( is_numeric( $arg1 ))
    if ( empty( $arg2 ))
        include "context/batch/view.inc";
    else
        include "context/batch/$arg2.inc";
else
    include "context/batch/$arg1.inc";



//-------------------------------------------------------------------------------------------
//      Form CRUD functions


/**
 *  batch edit form
 */
function _batch_form_content()
{
    $form_html = '';
    $tags_select_options = tags_select_options();
    
    $form_html .= input_text( 'Title' );
    $form_html .= textarea( 'Description' );
    
    //      Batch Tags
    if ( !empty( $tags_select_options ))
        $form_html .= multiple_select( 'Tag(s)', $tags_select_options );
    $form_html .= input_text( 'New Tag(s)', array( 'input_attributes' => array( 'help-inline' => "Multiple tags must be comma-separated <code>,</code>" )));
    
    //      Prepare multigroup data
    $envs_arr = db_fetchall( "SELECT * FROM `environment` ORDER BY `id_server` DESC, `id_environment` DESC" );
    $envs_options_arr = array();
    if ( !empty( $envs_arr ))
        foreach( $envs_arr as $env )
            $envs_options_arr[ $env[ 'id_environment' ]] = db_popfetch( "SELECT `title` FROM `server` WHERE `id_server` = ?", array( $env[ 'id_server' ])) ." : ". $env[ 'title' ] ." (". $env[ 'namespace' ] .")";
            //$envs_options_arr[ $env[ 'id_environment' ]] = db_popfetch( "SELECT `title` FROM `server` WHERE `id_server` = ?", array( $env[ 'id_server' ])) ." : ". $env[ 'title' ] ." (". $env[ 'namespace' ] .") [". $env[ 'id_environment' ] ."]";
    
    //      Multigroup : Batch Steps
    $form_html .= "<hr>";
    $form_html .= multigroup( "Batch Steps", array(
        array(
            'func' => 'input_text',
            'args' => array( "Step number", array( 'input_attributes' => array( 'help-inline' => "<small class='mute'>Optional : enter a number to specify the order of execution</small>" ))),
        ),
        array(
            'func' => 'input_text',
            'args' => array( "Step Title" ),
        ),
        array(
            'func' => 'select',
            'args' => array( "Environment", $envs_options_arr, array( 'select_attributes' => array( 'preserve_numeric_keys' => true ))),
        ),
        array(
            'func' => 'input_text',
            'args' => array( "Command", array( 'input_attributes' => array(
                'help-inline' => "<small class='mute'>(Autocompletion search)</small>",
                'class' => "autocompletion-command span5",
            ))),
        ),
        
        //      @evol 2013/07/29 19:31:54 - offer to select a folder based on tag to cd before executing this step ?
        //array(
        //    'func' => 'input_text',
        //    'args' => array( "Folder", array( 'input_attributes' => array( 'help-inline' => "<small class='mute'>Optional : enter a number to specify steps order of execution</small>" ))),
        //),
    ));
    
    return $form_html;
}



/**
 *  batch validation
 */
function _batch_form_validate( &$submit_handler_params )
{
    $return = array();
    
    //      debug
    //krumo( $_POST );
    
    //      Quick sanitization
    $posted = $_POST;
    form_recursive_sanitization( $posted );
    
    //      Save sanitized values for render_form()
    $return[ 'sanitized_values' ] = $posted;
    
    //      debug
    //krumo( $posted );
    
    //      Error checking
    if ( !empty( $posted[ 'title' ]))
    {
        //      Test if existing
        if ( $submit_handler_params[ 'is_new' ])
        {
            $existing_test = db_fetch( 'SELECT * FROM batch WHERE `title` LIKE ?', array( $posted[ 'title' ]));
            if ( !empty( $existing_test[ 'title' ]))
                $return[ 'errors' ] = alert( "It seems this <code>title</code> is already taken : ". $existing_test[ 'title' ], 'warning' );
        }
        
        //      Are we still good to proceed ?
        if ( empty( $return[ 'errors' ]))
        {
            //      Prepare 'batch' table values
            $submit_handler_params[ 'values' ] = array(
                'title' => $posted[ 'title' ],
                'description' => $posted[ 'description' ],
                'datestamp' => datestamp(),
            );
            
            //      Multigroup "Batch steps" mapping
            $batch_steps_mapping = array(
                'multifield-batch-steps-step-number' => 'n',
                'multifield-batch-steps-step-title' => 'title',
                'multifield-batch-steps-environment' => 'id_environment',
                'multifield-batch-steps-command' => 'cmd',
            );
            
            foreach( $posted as $name => $multivalues )
                if ( substr( $name, 0, 22 ) == 'multifield-batch-steps' )
                    foreach( $multivalues as $group => $value )
                        if ( !empty( $value ))
                            $submit_handler_params[ 'values_batch_steps' ][ intval( str_replace( 'multigroup-', '', $group ))][ $batch_steps_mapping[ $name ]] = $value;
            
            //      Tags preparation
            $submit_handler_params[ 'tags' ][ 'batch' ][ '#0' ][ 'tags' ] = clean_values_tags_select_options( $posted[ 'tag-s-' ]);
            $submit_handler_params[ 'tags' ][ 'batch' ][ '#0' ][ 'new_tags' ] = explode( ',', $posted[ 'new-tag-s-' ]);
        }
    }
    else
        $return[ 'errors' ][] = "Missing 'title'";
    
    //      debug
    //krumo( '_batch_form_validate() - end $submit_handler_params' );
    //krumo( $submit_handler_params );
    
    return $return;
}



/**
 *  Submit handler
 */
function _batch_form_submit( $params )
{
    $return = '';
    $error_flag = false;
    
    //      debug
    //return kpr( $params );
    //krumo( $params );
    
    //      @see _batch_form_validate()
    if ( !empty( $params[ 'values' ]))
    {
        //      Creation
        if ( $params[ 'is_new' ])
            $id_batch = insert_if_no_existing_record( 'batch', $params[ 'values' ], array( 'unique_field_name' => 'title' ));
        
        //      Edition
        else if ( $params[ 'id_batch' ])
            $id_batch = db_update( 'batch', $params[ 'values' ], array( 'id' => $params[ 'id_batch' ]));
        
        //      Check
        if ( $id_batch )
        {
            //      Feedback
            $return .= alert( "batch '". $params[ 'values' ][ 'title' ] ."' has been saved <code>id_batch = $id_batch </code>", 'success' );
            
            //      Prepare Tags : replace the key for helper function
            $params[ 'tags' ][ 'batch' ][ $id_batch ] = $params[ 'tags' ][ 'batch' ][ '#0' ];
            unset( $params[ 'tags' ][ 'batch' ][ '#0' ]);
            
            //      Batch steps
            if ( !empty( $params[ 'values_batch_steps' ]))
                foreach( $params[ 'values_batch_steps' ] as $i => $step )
                    if ( !empty( $step[ 'id_environment' ]) && !empty( $step[ 'cmd' ]))
                    {
                        //      Step number (order of execution)
                        $number = $i;
                        if ( !empty( $step[ 'n' ]))
                            $number = intval( $step[ 'n' ]);
                        
                        //      Convert command field autocompletion into 'id_command'
                        //      Format: "Title of command [123]", where 123 is the 'id_command'
                        //      Note : this sets a Warning - Strict standards: Only variables should be passed by reference
                        $id_command = end( explode( '[', substr( $step[ 'cmd' ], 0, -1 )));
                        
                        $id_batch_step = insert_or_update(
                            'batch_step',
                            array(
                                'title' => $step[ 'title' ],
                                'step' => $number,
                                'id_batch' => $id_batch,
                                'id_command' => $id_command,
                                'id_environment' => $step[ 'id_environment' ],
                            ),
                            array(
                                'detect_duplicate_query' => "SELECT `id_batch_step` FROM `batch_step`
                                    WHERE `id_batch` = $id_batch
                                    AND `id_environment` = ". $step[ 'id_environment' ] ."
                                    AND `id_command` = $id_command"
                                /*
                                'detect_duplicate_query' => "SELECT `id_batch_step` FROM `batch_step`
                                    WHERE `title` LIKE '". addcslashes( $values[ 'title' ], "'" ) ."'
                                    AND `id_batch` = $id_batch
                                    AND `step` = $number
                                    AND `id_environment` = $id_environment
                                    AND `id_command` = $id_command"
                                */
                            )
                        );
                    }
        }
        
        //      Feedback error
        else
        {
            $return .= alert( "Error : cannot save the batch '". $params[ 'values' ][ 'title' ] ."'. That's all I know.", 'error' );
            $error_flag = true;
        }
    }
    
    //      Set Tags
    set_tags( $params[ 'tags' ]);
    
    return $return;
}


