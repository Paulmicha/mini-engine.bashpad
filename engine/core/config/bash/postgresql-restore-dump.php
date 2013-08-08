<?php

$command = array(
    
    'title' => "PostgreSQL : Restore 'targzipped' Dump",
    'description' => "Requires setup for using command 'pg_restore' without password.",
    'tags' => array( "PostgreSQL" ),
    
    'command' => 'cd [folder:db dumps]
tar xzf [dump_filename].sql.tar.gz
pg_restore --username "[username:pgsql]" --dbname "[db_name:pgsql]" -h localhost [dump_filename].sql.tar
rm [dump_filename].sql.tar',

);

