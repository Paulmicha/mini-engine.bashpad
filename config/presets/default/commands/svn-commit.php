<?php

$command = array(
    
    'title' => "SVN commit",
    'description' => "Use absolute path",
    'command' => "svn commit [dir] -m \"[message]\"",
    'tags' => array( "SVN" ),
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'dir',
                'configuration' => 'raw',
                'default_value' => '[folder:project root]',
            ),
            '1' => array(
                'namespace' => 'message',
                'configuration' => 'raw',
                'default_value' => 'Routine commit',
            ),
        ),
    ),
);


