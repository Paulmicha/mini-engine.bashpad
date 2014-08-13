<?php

$extensions[ 'db' ] = array(
    'settings' => array(
        
        //      This 'default' key must always exist,
        //      even for multi-DB setups.
        'default' => array(
            
            //      Optional (defaults)
            //      @see get_db_config()
            //'driver' => 'mysql',
            //'pdo_attr' => array(),
            //'pdo_named_params' => false,
            //'host' => 'localhost',
            //'port' => '',
            
            'dbname' => '',
            'user' => '',
            'pass' => '',
        ),
        
        //      To add DBs, simply add a key
        //      ex:
        //$extensions[ 'db' ][ 'settings' ][ 'my_db1' ] = array(
        //    'dbname' => 'mydb',
        //    'user' => 'mylogin',
        //    'pass' => 'mypass',
        //);
    ),
    
    //      This allows multi-db "hot swaps"
    //      ex:
    'active' => 'default',
);
