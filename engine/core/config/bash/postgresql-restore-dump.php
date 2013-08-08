<?php

$command = array(
    
    'title' => "PostgreSQL : Restore 'targzipped' Dump",
    'description' => "Requires setup for using command 'pg_restore' without password.",
    'tags' => array( "PostgreSQL" ),
    
    'command' => 'cd [folder:db dumps]
tar xzf [dump_filename].sql.tar.gz
pg_restore --username "[username:pgsql]" --dbname "[db_name:pgsql]" -h localhost [filename].sql.tar
rm [filename].sql.tar',
    
    //      @evol 2013/08/08 11:31:05 - new default token todo
    //'data' => array(
    //    'tokens' => array(
    //        '0' => array(
    //            'namespace' => 'filename',
    //            'configuration' => 'options',
    //            'preprocess' => 'preprocess_token_config__filename',
    //        ),
    //    ),
    //),

);


