<?php

/**
 *  @file
 *  Commands - view
 *  
 *  @path /sytem/command/123
 */

//      Init
$head_page_title = "$command_title summary";

//      Command data
$tags_command = get_tags( 'command', $arg2 );
$command_data = db_fetch( "SELECT * FROM `command` WHERE `id_command` = ?", array( $arg2 ));

//      Display command on top
$content .= view_source( $command_data[ 'command' ], 'bash' );
$content .= "<hr>";

//      List data
$command_data_list_arr = array(
    "Tags" => view_tags( $tags_command ),
    "description" => $command_data[ 'description' ],
    //"Command" => "<pre>". $command_data[ 'command' ] ."</pre>",
);
//$content .= dl( $command_data_list_arr );

//      Tokens summary
$tokens_html = '';
if ( !empty( $command_data[ 'data' ]))
{
    $command_data[ 'data' ] = safe_unserialize( $command_data[ 'data' ]);
    
    //      debug
    //krumo( $command_data[ 'data' ]);
    
    //$tokens_list_arr = array();
    //foreach( $command_data[ 'data' ] as $token )
    //    $tokens_list_arr[  ] = ;
    
    $lines = array();
    if ( !empty( $command_data[ 'data' ][ 'tokens' ]))
    {
        $mapping_cfg = array(
            'raw' => "Raw string argument",
            'options' => "Options list (unique selection)",
            'multiple_options' => "Options list (multiple selection)",
            'folder' => "Folder",
        );
        
        foreach( $command_data[ 'data' ][ 'tokens' ] as $e )
        {
            if ( !empty( $e[ 'options' ]))
                $e[ 'options' ] = '<pre>'. $e[ 'options' ] .'</pre>';
            
            $lines[] = array(
                '<code>['. $e[ 'namespace' ] .']</code>',
                $mapping_cfg[ $e[ 'configuration' ]],
                $e[ 'default_value' ],
                $e[ 'options' ],
            );
        }
        
        $tokens_html .= table( $lines, array( 'header' => array(
            "Namespace",
            "Type of configuration",
            "Default value",
            "List of options",
        )));
    }
}


//      View in 2 columns
$content .= grid( array(
    array( "<h2>Details</h2>", "<h2>Tokens</h2>" ),
    array( dl( $command_data_list_arr ), $tokens_html ),
));


