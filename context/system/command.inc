<?php

/**
 *  @file
 *  Commands dispatcher
 *  
 *  @path system/command
 *  @path system/command/presets
 *  @path system/command/add
 *  @path system/command/123
 *  @path system/command/123/<operation>
 */

//      Init
$head_page_title = "Manage Commands";



//-------------------------------------------------------------------------------------------
//      Local menus


$arg2 = check_plain( arg( 2 ));
$arg3 = check_plain( arg( 3 ));
$arg4 = check_plain( arg( 4 ));

$tab_items = array(
    array( "Commands list", 'system/command' ),
    array( "Add new command", 'system/command/add' ),
    //array( "Presets", 'system/command/presets' ),
);
$subtab_items = array();
$subtab_tabs = array();

if ( is_numeric( $arg2 ))
{
    $command_title = db_popfetch( "SELECT `title` FROM `command` WHERE `id_command` = ?", array( $arg2 ));
    //$tab_items[] = array( "Manage $command_title", 'system/command/'. $arg2, 'path_is_in_base_route' );
    $tab_items[ 0 ] = array( "Commands list", 'system/command', 'path_is_in_base_route' );
    
    $subtab_items[] = array( $command_title, 'system/command/'. $arg2 );
    $subtab_items[] = array( "<i class='icon-pencil'></i>&nbsp;Edit", 'system/command/'. $arg2 .'/edit' );
    //$subtab_items[] = array( "<i class='icon-dashboard'></i>&nbsp;Preview", 'system/command/'. $arg2 .'/preview' );
    //$subtab_items[] = array( "<i class='icon-dashboard'></i>&nbsp;Check compatibility", 'system/command/'. $arg2 .'/compatibility' );
    $subtab_items[] = array( "<i class='icon-trash'></i> Delete", 'system/command/'. $arg2 .'/delete' );
}

$content .= nav_tabs( $tab_items );

if ( !empty( $subtab_items ))
    $content .= nav_pills( $subtab_items );



//-------------------------------------------------------------------------------------------
//      Dispatch


//      Default handler
if ( empty( $arg2 ))
    include "context/system/command/list.inc";

//      Including corresponding files from paths
else if ( is_numeric( $arg2 ))
    if ( empty( $arg3 ))
        include "context/system/command/view.inc";
    else
        include "context/system/command/$arg3.inc";
else
    include "context/system/command/$arg2.inc";



//-------------------------------------------------------------------------------------------
//      Form CRUD functions


/**
 *  command edit form
 */
function _command_form_content()
{
    $form_html = '';
    $tags_select_options = tags_select_options();
    
    $form_html .= input_text( 'Title', array( 'input_attributes' => array( 'class' => "span8" )));
    $form_html .= textarea( 'Description', array( 'textarea_attributes' => array( 'class' => "span8" )));
    
    //      Environment Tags
    if ( !empty( $tags_select_options ))
        $form_html .= multiple_select( 'Tag(s)', $tags_select_options );
    $form_html .= input_text( 'New Tag(s)', array( 'input_attributes' => array( 'help-inline' => "Multiple tags must be comma-separated <code>,</code>" )));
    
    //      The command
    $form_html .= "<hr>";
    $form_html .= textarea( 'Command', array( 'textarea_attributes' => array(
        'rows' => "6",
        'class' => "span8",
        'help-block' => "Use tokens to insert configurable values, ex: <code>[custom_token_namespace]</code> (use repeatable fields below)<br>
            Default tokens :<br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[datestamp]</code>, which will print <code>". datestamp_token() ."</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[env]</code>, which will print for example <code>Project-Title-ENV</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[folder:Project root dir]</code>, which will print for example <code>/my/folder/path</code> <small>Note : the argument is a <strong>Tag title</strong>.<br>It is used to match the 1<sup>st</sup> folder tagged with it in the environment in which the command will be executed.</small><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[username:mysql]</code> <small>Note : the string as argument is a <strong>Credential Type</strong>.<br>It is used to match the 1<sup>st</sup> username in the environment in which the command will be executed.</small><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[password:mysql]</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[db_name:mysql]</code><br>
            <i class='icon-long-arrow-right'></i>&nbsp;<code>[dump_filename]</code> : unique name produced by <code>[env]-[datestamp]</code>, reused between steps - allows chaining DB dumps creation and restoration in the same batch.
            ",
    )));
    
    //      Custom Tokens
    $form_html .= "<hr>";
    $form_html .= multigroup( "Custom tokens", array(
        array(
            'func' => 'input_text',
            'args' => array( "Token Namespace", array(
                'input_attributes' => array(
                    'help-inline' => "Must match those inside the 'Command' textarea",
                ),
                'prepend' => '[',
                'append' => ']',
            )),
        ),
        array(
            'func' => 'select',
            'args' => array(
                "Type of configuration",
                array(
                    '' => "---",
                    'raw' => "Raw string value",
                    'options' => "Options list (unique selection)",
                    'multiple_options' => "Options list (multiple selection)",
                ),
                array( 'select_attributes' => array( 'help-inline' => "Used to determine which form element to use<br>to set the value replacing the token<br>(in the 'Batch' > 'Launch Operations' form)" ))
            ),
        ),
        array(
            'func' => 'textarea',
            'args' => array( "List of options", array( 'textarea_attributes' => array( 'help-inline' => "<span class='muted'>(if applicable)</span><br>Use the following format, one option per line :<br><i class='icon-long-arrow-right'></i>&nbsp;<code>value|Option label</code>" ))),
        ),
        array(
            'func' => 'textarea',
            'args' => array( "Default value", array( 'input_attributes' => array( 'help-inline' => "<span class='muted'>(optional)</span>" ))),
        ),
    ));
    
    return $form_html;
}



/**
 *  command validation
 */
function _command_form_validate( &$submit_handler_params )
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
    if ( !empty( $posted[ 'command' ]) && !empty( $posted[ 'title' ]))
    {
        //      Test if existing
        if ( $submit_handler_params[ 'is_new' ])
        {
            $existing_test = db_fetch( 'SELECT * FROM command WHERE `title` LIKE ?', array( $posted[ 'title' ]));
            if ( !empty( $existing_test[ 'title' ]))
                $return[ 'errors' ] = alert( "It seems this <code>title</code> is already taken : ". $existing_test[ 'title' ], 'warning' );
        }
        
        //      Are we still good to proceed ?
        if ( empty( $return[ 'errors' ]))
        {
            //      Prepare 'Command' table values
            $submit_handler_params[ 'values' ] = array(
                'title' => $posted[ 'title' ],
                'description' => $posted[ 'description' ],
                'command' => $posted[ 'command' ],
                'datestamp' => datestamp(),
            );
            
            //      Multigroup "Custom Tokens" mapping
            $tokens_mapping = array(
                'multifield-custom-tokens-token-namespace' => 'namespace',
                'multifield-custom-tokens-type-of-configuration' => 'configuration',
                'multifield-custom-tokens-list-of-options' => 'options',
                'multifield-custom-tokens-default-value' => 'default_value',
            );
            foreach( $posted as $name => $multivalues )
                if ( substr( $name, 0, 24 ) == 'multifield-custom-tokens' )
                    foreach( $multivalues as $group => $value )
                        if ( !empty( $value ))
                            $submit_handler_params[ 'values' ][ 'data' ][ 'tokens' ][ intval( str_replace( 'multigroup-', '', $group ))][ $tokens_mapping[ $name ]] = $value;
            
            //      Serialize data
            $submit_handler_params[ 'values' ][ 'data' ] = safe_serialize( $submit_handler_params[ 'values' ][ 'data' ]);
            
            //      Tags preparation
            $submit_handler_params[ 'tags' ][ 'command' ][ '#0' ][ 'tags' ] = clean_values_tags_select_options( $posted[ 'tag-s-' ]);
            $submit_handler_params[ 'tags' ][ 'command' ][ '#0' ][ 'new_tags' ] = explode( ',', $posted[ 'new-tag-s-' ]);
        }
    }
    else
        $return[ 'errors' ][] = "Missing 'title' or 'command'";
    
    return $return;
}



/**
 *  Submit handler
 */
function _command_form_submit( $params )
{
    $return = '';
    $error_flag = false;
    
    //      debug
    //return kpr( $params );
    //krumo( $params );
    
    //      @see _command_form_validate()
    if ( !empty( $params[ 'values' ]))
    {
        //      Creation
        if ( $params[ 'is_new' ])
            $id_command = insert_if_no_existing_record( 'command', $params[ 'values' ], array( 'unique_field_name' => 'title' ));
        
        //      Edition
        else if ( $params[ 'id_command' ])
            $id_command = db_update( 'command', $params[ 'values' ], array( 'id' => $params[ 'id_command' ]));
        
        //      Check
        if ( $id_command )
        {
            //      Feedback
            $return .= alert( "Command '". $params[ 'values' ][ 'title' ] ."' has been saved <code>id_command = $id_command </code>", 'success' );
            
            //      Prepare Tags : replace the key for helper function
            $params[ 'tags' ][ 'command' ][ $id_command ] = $params[ 'tags' ][ 'command' ][ '#0' ];
            unset( $params[ 'tags' ][ 'command' ][ '#0' ]);
        }
        
        //      Feedback error
        else
        {
            $return .= alert( "Error : cannot save the command '". $params[ 'values' ][ 'title' ] ."'. That's all I know.", 'error' );
            $error_flag = true;
        }
    }
    
    //      Set Tags
    set_tags( $params[ 'tags' ]);
    
    return $return;
}


