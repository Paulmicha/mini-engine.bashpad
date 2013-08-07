<?php

/**
 *  @file
 *  command - edit
 *  @path /sytem/command/1/edit
 */

//      Init
$head_page_title = "Edit command $command_title";

//      Loading data
$command_data = db_fetch( "SELECT * FROM `command` WHERE `id_command`=$arg2" );
if ( empty( $command_data ))
    $content .= alert( "command not found.", 'error' );
else
{
    $head_page_title = "Manage ". $command_data[ 'title' ];
    
    //      Multiselect : Tag(s) mapping preparation
    $tags_command = get_tags( 'command', $arg2 );
    $tags_command_arr = array();
    foreach( $tags_command as $id_tag )
        $tags_command_arr[] = "id-$id_tag";
    
    //      Multigroup : Custom tokens mapping preparation
    $custom_tokens_mapping = array();
    if ( !empty( $command_data[ 'data' ]))
    {
        $command_data[ 'data' ] = safe_unserialize( $command_data[ 'data' ]);
        
        if ( !empty( $command_data[ 'data' ][ 'tokens' ]))
            foreach( $command_data[ 'data' ][ 'tokens' ] as $i => $c )
            {
                $custom_tokens_mapping[ 'multifield-custom-tokens-token-namespace' ][ 'multigroup-'. $i ] = $c[ 'namespace' ];
                $custom_tokens_mapping[ 'multifield-custom-tokens-type-of-configuration' ][ 'multigroup-'. $i ] = $c[ 'configuration' ];
                $custom_tokens_mapping[ 'multifield-custom-tokens-list-of-options' ][ 'multigroup-'. $i ] = $c[ 'options' ];
                $custom_tokens_mapping[ 'multifield-custom-tokens-default-value' ][ 'multigroup-'. $i ] = $c[ 'default_value' ];
            }
    }
    
    //      Mapping
    $form_values = array(
        'id_command' => $command_data[ 'id_command' ],
        'title' => $command_data[ 'title' ],
        'description' => $command_data[ 'description' ],
        'tag-s-' => $tags_command_arr,
        'command' => $command_data[ 'command' ],
        'multigroups' => array(
            'custom_tokens' => $custom_tokens_mapping,
        ),
    );
    
    //      Render form
    $content .= render_form(
        form_set_values( _command_form_content(), $form_values ),
        '_command_form_submit',
        array(
            'validation_handler' => '_command_form_validate',
            'submit_handler_params' => array( 'id_command' => $command_data[ 'id_command' ]),
        )
    );
}


