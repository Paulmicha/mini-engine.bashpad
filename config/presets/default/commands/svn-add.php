<?php

$command = array(
    
    'title' => "SVN add",
    'description' => "Use absolute path. The default option - <code>--force</code> - will add all new files recursively",
    'command' => "svn add [dir] [options]",
    'tags' => array( "SVN" ),
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'dir',
                'configuration' => 'raw',
                'default_value' => '[folder:project root]',
            ),
            '1' => array(
                'namespace' => 'options',
                'configuration' => 'raw',
                'default_value' => '--force',
            ),
        ),
    ),
);


