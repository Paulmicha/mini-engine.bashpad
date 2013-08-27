<?php

$command = array(
    
    'title' => "Symfony2 install",
    'description' => "Symfony2 install",
    'command' => "php composer.phar create-project symfony/framework-standard-edition [folder:project root] [version]",
    'tags' => array( "Symfony", "Initialization" ),
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'version',
                'configuration' => 'raw',
                'default_value' => '2.3.3',
            ),
        ),
    ),
);


