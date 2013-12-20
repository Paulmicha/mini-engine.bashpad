<?php

$command = array(
    
    'title' => "PostgreSQL : Drop schema & all tables in it",
    'description' => "Requires setup for using command 'psql' without password.",
    'tags' => array( "PostgreSQL", "Database operations" ),
    
    'command' => 'psql [db_name:pgsql] -c "DROP SCHEMA [schema] CASCADE"',
    
    'data' => array(
        'tokens' => array(
            '0' => array(
                'namespace' => 'schema',
                'configuration' => 'raw',
                'default_value' => '',
            ),
        ),
    ),
);


