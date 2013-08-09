<?php

$command = array(
    
    'title' => "Wget",
    'description' => "Use absolute paths for ",
    'command' => "cd [destination_folder] && wget [source] [wget_options]",
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'source',
                'configuration' => 'raw',
                'default_value' => '',
            ),
            '1' => array(
                'namespace' => 'destination_folder',
                'configuration' => 'raw',
                'default_value' => '',
            ),
            '2' => array(
                'namespace' => 'wget_options',
                'configuration' => 'raw',
                'default_value' => '--no-verbose',
            ),
        ),
    ),
);


