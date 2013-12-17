<?php

/**
 *  Example server definition (with its environments)
 *  This kind of preset allows to quickly restore a "clean" mini-engine, with custom server configurations in place
 */

$server = array(
    
    //      Note : we need a GUID to handle import/update without creating duplicates
    //      Proposition : concatenation of hostname + ssh-username
    //'id_server' => 1,
    
    'title' => "Server Name",
    'description' => "This kind of preset allows to quickly restore a \"clean\" mini-engine, with custom server configurations in place",
    'hostname' => '192.168.123.123',
    'data' => array(
        'ssh' => array(
            'u' => 'ssh-username',
            'p' => 'ssh-password',
            
            //      Optional
            //'k' => 'ssh-private-key',
            //'port' => 'ssh-port',
        ),
    ),
);



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
        //      Proposition : concatenation of server id + namespace
        //'id_environment' => 1,
        
        //      Hard value
        //      @todo 2013/12/17 14:16:51 - write function to get IDs from GUIDs (per entity type)
        //      -> ex :
        //      $server_id = guid_to_id( 'server', array( 'hostname', 'ssh-username' ));
        'id_server' => 1,
        
        //      Automatically generable :
        //      concatenation of str_clean( title ) + '_' +  last item in project dir path
        'namespace' => 'project-name_www',
        
        //      @evol 2013/12/17 14:32:43 - split into 2 fields : project name + instance name
        //      (because we often instanciate several versions of the same project in a single server)
        //      #NamingConvention
        'title' => "Project Name - Instance Name",
        
        'url' => 'http://192.168.123.123/project-name/www',
        
        //      Optional
        'description' => "My project instance description.",
        
        //      @todo 2013/12/17 14:30:49
        'data' => array(
            
            //      @evol 2013/12/17 14:32:01
            //'repository-url'
            //'repository-branch'
        ),
    ),
);



