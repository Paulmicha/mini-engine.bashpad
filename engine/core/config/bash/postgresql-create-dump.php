<?php

$command = array(
    
    'title' => "PostgreSQL : Create 'targzipped' Dump",
    'description' => "Requires setup for using command 'pg_dump' without password.",
    'tags' => array( "PostgreSQL", "Database operations" ),
    
    'command' => "cd [folder:db dumps]
pg_dump -i -U [username:pgsql] -h localhost -F t [db_name:pgsql] > [dump_filename].sql.tar
tar czf [dump_filename].sql.tar.gz [dump_filename].sql.tar
rm [dump_filename].sql.tar",

);


