<?php

$command = array(
    
    'title' => "Copy",
    'description' => "Use absolute paths",
    'command' => "cp [source] [destination]",
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'source',
                'configuration' => 'raw',
                'default_value' => '',
            ),
            '1' => array(
                'namespace' => 'destination',
                'configuration' => 'raw',
                'default_value' => '',
            ),
        ),
    ),
);


