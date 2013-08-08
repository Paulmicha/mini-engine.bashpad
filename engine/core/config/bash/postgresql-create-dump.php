<?php

$command = array(
    
    'title' => "PostgreSQL : Create 'targzipped' Dump",
    'description' => "Requires setup for using command 'pg_dump' without password.",
    'tags' => array( "PostgreSQL" ),
    
    'command' => "cd [folder:db dumps]
pg_dump -i -U [username:pgsql] -h localhost -F t [db_name:pgsql] > [env]-[datestamp].sql.tar
tar czf [env]-[datestamp].sql.tar.gz [env]-[datestamp].sql.tar
rm [env]-[datestamp].sql.tar",

);


