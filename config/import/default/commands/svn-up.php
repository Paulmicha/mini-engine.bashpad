<?php

$command = array(
    
    'title' => "SVN update",
    'description' => "Use absolute path",
    'command' => "cd [dir] && svn up",
    'tags' => array( "SVN" ),
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'dir',
                'configuration' => 'raw',
                'default_value' => '[folder:project root]',
            ),
        ),
    ),
);


