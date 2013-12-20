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
    array( "Presets", 'batch/presets', 'path_is_in_base_route' ),
    array( "Archives", 'batch/archives', 'path_is_in_base_route' ),
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

//      Single Batch paths
else if ( is_numeric( $arg1 ))
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

//      Batch operations
else if ( is_numeric( $arg1 ))
    if ( empty( $arg2 ))
        include "context/batch/view.inc";
    else
        include "context/batch/$arg2.inc";

//      Batch execution archives
else if ( $arg1 == 'archives' && is_numeric( $arg2 ))
    include "context/batch/archive-view.inc";

//      Presets
else if ( $arg1 == 'presets' && is_numeric( $arg2 ))
    include "context/batch/presets-render.inc";

//      Default actions
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
        
        "<p>&nbsp;</p>",
        //"<hr>",
        //"<p>Specify this step's operation :</p>",
        
        //      Left tabs - opening wrapper div
        "<div class='tabbable tabs-left'>",
        
        //      Left tabs - Tabs controls (titles)
        "<ul class='nav nav-tabs'>
            <li class='active'><a href='#left-tab--ssh--multigroup-0' data-toggle='tab'><i class='icon-cogs'></i>&nbsp;SSH command</a></li>
            <li><a href='#left-tab--sftp--multigroup-0' data-toggle='tab'><i class='icon-exchange'></i>&nbsp;SFTP file transfert</a></li>
            <li><a href='#left-tab--raw--multigroup-0' data-toggle='tab'><i class='icon-ambulance'></i>&nbsp;Raw Bash input</a></li>
            
            <!--
            //      @evol 2013/08/27 13:31:45 - we could use more, for example :
            //      Setting custom Bash raw input,
            //      Specific interfaces, such as Drupal installation (distribution, presets / lists of modules, etc.), or Symfony (lists of Bundles, etc.)
            <li><a href='#left-tab--presets--multigroup-0' data-toggle='tab'><i class='icon-rocket'></i>&nbsp;Specific presets</a></li>
            -->
            
        </ul>",
        
        //      Left tabs - opening Tab Contents wrapper div
        "<div class='tab-content'>",
        
        //      Left tabs - Tab Content : Options to execute SSH Command
        "<div class='tab-pane active' id='left-tab--ssh--multigroup-0'>",
        "<p>Select a stored SSH command :</p>",
        
        array(
            'func' => 'input_text',
            'args' => array( "Command", array( 'input_attributes' => array(
                'help-inline' => "<small class='mute'>(Autocompletion search)</small>",
                'class' => "autocompletion-command span5",
                
                //      Turning off Browsers' default autocompletion, because it interferes with our own.
                'autocomplete' => 'off',
            ))),
        ),
        "</div>",
        
        //      Left tabs - Tab Content : transfer a file in SFTP
        "<div class='tab-pane' id='left-tab--sftp--multigroup-0'>",
        "<p>Transfer a file :</p>",
        
        array(
            'func' => 'input_text',
            'args' => array( "SFTP Source Filepath", array( 'input_attributes' => array( 'help-inline' => "<small class='mute'>Supports tokens - e.g. <code>[folder:db dumps]/[dump_filename]</code></small>" ))),
        ),
        array(
            'func' => 'select',
            'args' => array( "SFTP Destination Environment", $envs_options_arr, array( 'select_attributes' => array( 'preserve_numeric_keys' => true ))),
        ),
        array(
            'func' => 'input_text',
            'args' => array( "SFTP Destination Filepath", array( 'input_attributes' => array( 'help-inline' => "<small class='mute'>Destination Environment Filepath</small><br><small class='mute'>Supports tokens - e.g. <code>[folder:db dumps]/[dump_filename]</code></small>" ))),
        ),
        "</div>",
        
        //      Left tabs - Tab Content : Specific presets
        //"<div class='tab-pane' id='left-tab--presets--multigroup-0'>",
        //"<p>Specific presets :</p>",
        //"<p>@todo 2013/08/27 13:56:09</p>",
        //"<p>Specific interfaces, such as Drupal installation (distribution, presets / lists of modules, etc.), or Symfony (lists of Bundles, etc.)</p>",
        //"</div>",
        
        //      Left tabs - Tab Content : Raw Bash input
        "<div class='tab-pane' id='left-tab--raw--multigroup-0'>",
        "<p>Raw Bash Input (know what you're doing)</p>",
        array(
            'func' => 'textarea_raw',
            'args' => array( "Raw Bash Input", array( 'class' => 'span12', 'rows' => 6, )),
        ),
        "<p>Supports Default tokens :<br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[datestamp]</code>, which will print <code>". datestamp_token() ."</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[env]</code>, which will print for example <code>Project-Title-ENV</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[folder:project root]</code>, which will print for example <code>/my/folder/path</code> <small>Note : the argument is a <strong>Tag title</strong> (case insensitive).<br>It is used to match the 1<sup>st</sup> folder tagged with it in the environment in which the command will be executed.</small><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[username:mysql]</code> <small>Note : the string as argument is a <strong>Credential Type</strong>.<br>It is used to match the 1<sup>st</sup> username in the environment in which the command will be executed.</small><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[password:mysql]</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[db_name:mysql]</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[dump_filename]</code> : unique name produced by <code>[env]-[datestamp]</code>, reused between steps - allows chaining DB dumps creation and restoration in the same batch.</p>",
        "</div>",
        
        //      Left tabs - closing Tab Contents wrapper div
        "</div>",
        
        //      Left tabs - closing wrapper div
        "</div>",
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
    
    //      Sanitization seems to add HTMLENTITIES, and we can edit Bash commands...
    //      so, no "sanitization"
    $posted = $_POST;
    //form_recursive_sanitization( $posted );
    
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
    //krumo( '_batch_form_submit() - $params' );
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
                        //$id_command = end( explode( '[', substr( $step[ 'cmd' ], 0, -1 )));
                        $_parts_arr = explode( '[', substr( $step[ 'cmd' ], 0, -1 ));
                        $id_command = end( $_parts_arr );
                        
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


