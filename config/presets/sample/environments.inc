<?php

/**
 *  @file
 *  Sample environment
 *  
 *  @requires Sample server import in config/presets/sample/server.inc
 */

//      Here is an example of the data expected for a functional environment (usually corresponding to a project) :
/*
Project Name - 192.168.123.123 / WWW - URL : http://192.168.123.123/project-name/www
Project Name - 192.168.123.123 / WWW - Project Dir : /var/www/project-name/www
Project Name - 192.168.123.123 / WWW - DB Dumps Dir : /var/custom/project-name/www/dumps
Project Name - 192.168.123.123 / WWW - MySQL : db_user / db_pass @ db_name
*/

$environments = array(
    
    //      For each environment (usually corresponding to a project),
    //      we must specify the link to its server with its custom GUID :
    //      as of 2013/12/17 14:03:55 this is a concatenation of hostname + ssh-username
    array(
        
        //      Note : we need a GUID to handle import/update without creating duplicates
        //      Proposition : concatenation of server GUID + namespace
        //'id_environment' => 1,
        //      @see below
        //'guid' => '192.168.123.123,ssh-username,project-name/www',
        
        //      Hard value
        //      @todo 2013/12/17 14:16:51 - anticipate export feature : write function to get IDs from GUIDs (per entity type) ?
        //      -> ex :
        //      $server_id = guid_to_id( 'server', array( '192.168.123.123', 'ssh-username' ));
        //      A bit long to do : would need a mapping utility to "reverse-engineer" the GUID (from database).
        //      That utility would store per entity type how the concatenation was made,
        //      and could only work if the separator can be 'exploded'
        //      -> advanced evol : allow this utility to work with values from other tables.
        'id_server' => 1,
        
        //      Automatically generable :
        //      concatenation of str_clean( title ) + '/' + either one of : Instance Name (requires @evol 2013/12/17 14:32:43), or : last item in project dir path
        'namespace' => 'project-name/www',
        
        //      @evol 2013/12/17 14:32:43 - split into 2 fields : project name + instance name
        //      (because we often instanciate several versions of the same project in a single server)
        //      #NamingConvention
        'title' => "Project Name / Instance Name",
        
        'url' => 'http://192.168.123.123/project-name/www',
        
        //      Optional
        'description' => "My project instance description.",
        
        //      Misc. examples
        'data' => array(
            'credentials' => array(
                array(
                    'type' => 'mysql',
                    'db_name' => 'example_db_name',
                    'username' => 'example_db_login',
                    'password' => 'example_db_pass',
                    //'prefix' => null,
                    //'port' => null,
                ),
                array(
                    'type' => 'psql',
                    'db_name' => 'example_psql_name',
                    'username' => 'example_psql_login',
                    'password' => 'example_psql_pass',
                    'prefix' => 'example_prefix',
                ),
                array(
                    'type' => 'git',
                    'url' => 'https://github.com/Paulmicha/Mini-engine.git',
                    'username' => 'example_git_login',
                    'password' => 'example_git_pass',
                    //'branch' => 'master',
                    //'tag' => '1.0',
                ),
            ),
        ),
    ),
);

//      Prepare for DB insertion
//      @evol 2013/12/20 00:12:27 - automatic handling of preprocess to read / write from / to DB
//      @see [planned] mini-ORM
$environments[ 0 ][ 'guid' ] = guid( $environments[ 0 ], array( 'parts_arr' => array( '192.168.123.123,ssh-username', $environments[ 0 ][ 'namespace' ])));
$environments[ 0 ][ 'data' ] = safe_serialize( $environments[ 0 ][ 'data' ]);

//      Write to DB
$id = insert_or_update( 'environment', $environments[ 0 ], array( 'unique_field_name' => 'guid' ));

//      Feedback
$content .= "<hr>";
$content .= alert( "Sample environment processed - id_environment : <code>[$id]</code>" );

