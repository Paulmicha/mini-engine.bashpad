<?php

/**
 *  @file
 *  Bashpad misc. helpers
 *  
 *  Processes and helpers for the following associated entites :
 *      Server, Environment, Folder
 */


/**
 *  Simple TXT Parser
 *  
 *  Expected behavior :
 *      • 1 item per line
 *      • Comment line must start with ";"
 *      • Empty or invalid lines are skipped
 *      • Separators are " - ", " / " or " : " (require at least 1 white space before AND after)
 *      • Fragments are "trimmed" (surrounding whitespace stripped)
 *      • Case insensitive, whitespace-ignorant recognition of components - ex :
 *          ssh, url, project + dir | folder, (db) dump + dir | folder, mysql, psql | pgsql
 *  
 *  Example (repetitions matter) :
 *  
 *      Server Name - SSH : login / password @ hostname
 *      Project Title - Server Name / Instance Name - URL : http://staging.project.example.com/
 *      Project Title - Server Name / Instance Name - Project Dir : /var/www/project-title/www
 *      Project Title - Server Name / Instance Name - DB Dumps Dir : /var/custom/project-title/www/dumps
 *      Project Title - Server Name / Instance Name - MySQL : db_user / db_pass @ db_name (: port)
 *      Project Title - Server Name / Instance Name - PSQL : db_user / db_pass @ db_name (: port)
 *  
 *  @param String $filepath : text file path to parse
 *  @return Array : keyed by entity, formatted for DB Insertion
 */
function servers_and_envs_text_parse( $filepath )
{
    $structured_arr = array();
    
    if ( !empty( $filepath ) && file_exists( $filepath ))
    {
        $text = file_get_contents( $filepath );
        $file = fopen( $filepath, "r" );
        
        if ( $file )
        {
            while( !feof( $file ))
            {
                $line = fgets( $file );
                $line = trim( $line );
                
                if ( !empty( $line ) && substr( $line, 0, 1 ) != ';' )
                {
                    $fragments = servers_and_envs_line_tokenizer( $line );
                    
                    if ( !empty( $fragments ))
                    {
                        $entity = array();
                        $entity_type = '';
                        list( $designation, $value ) = $fragments;
                        
                        //      Servers
                        if ( count( $designation ) == 2 )
                        {
                            $entity_type = 'server';
                            $entity[ 'title' ] = $designation[ 0 ];
                            
                            switch( str_clean( $designation[ 1 ]))
                            {
                                case 'ssh' :
                                    
                                    if ( !empty( $value[ 2 ])) {
                                        $entity[ 'hostname' ] = $value[ 2 ];
                                    }
                                    
                                    $entity[ 'data' ][ 'ssh' ] = array(
                                        'u' => $value[ 0 ],
                                        'p' => $value[ 1 ],
                                    );
                                    
                                    break;
                                
                                case 'hostname' :
                                    
                                    if ( is_string( $value )) {
                                        $entity[ 'hostname' ] = $value;
                                    }
                                    
                                    break;
                            }
                        }
                        
                        //      Environments
                        else if ( count( $designation ) == 3 )
                        {
                            $entity_type = 'environment';
                            $entity[ 'project' ] = $designation[ 0 ];
                            $entity[ 'server' ] = $designation[ 1 ];
                            
                            if ( is_array( $designation[ 1 ]))
                            {
                                $entity[ 'server' ] = $designation[ 1 ][ 0 ];
                                $entity[ 'instance' ] = $designation[ 1 ][ 1 ];
                            }
                            
                            //$env_id = str_clean( $entity[ 'project' ] .'-'. $entity[ 'server' ] .'/'. $entity[ 'instance' ] );
                            
                            switch( str_clean( $designation[ 2 ]))
                            {
                                case 'url' :
                                    
                                    if ( is_string( $value )) {
                                        $entity[ 'url' ] = $value;
                                    }
                                    
                                    break;
                                
                                //      These must create new "folder" entities
                                case 'project-dir' :
                                case 'project-folder' :
                                    
                                    //$entity[ 'folders' ]
                                    
                                    break;
                                
                                //url, project + dir | folder, (db) dump + dir | folder, mysql, psql | pgsql
                            }
                        }
                        
                        //      All entities share that treatment
                        $guid = entity_guid( $entity_type, $entity );
                        
                        if ( !empty( $structured_arr[ $entity_type ][ $guid ])) {
                            $structured_arr[ $entity_type ][ $guid ] += $entity;
                        }
                        else {
                            $structured_arr[ $entity_type ][ $guid ] = $entity;
                        }
                    }
                }
            }
            
            fclose( $file );
        }
    }
    
    return $structured_arr;
}



/**
 *  Extract data into array from a single line
 */
function servers_and_envs_line_tokenizer( $line )
{
    $fragments = array();
    
    if ( !empty( $line ))
    {
        $separator_primary = ' : ';
        $separator_secondary = ' - ';
        $separator_tertiary = ' / ';
        
        $separator_credentials = '@';
        $separator_port = ':';
        
        $separation_primary = strpos( $line, $separator_primary );
        
        if ( !$separation_primary ) {
            continue;
        }
        
        $fragments[ 'primary' ][ 0 ] = substr( $line, 0, $separation_primary );
        $fragments[ 'primary' ][ 1 ] = substr( $line, $separation_primary + strlen( $separator_primary ));
        
        foreach( $fragments[ 'primary' ] as $i => $primary_segment )
        {
            $fragments[ 'secondary' ][ $i ] = explode( $separator_secondary, $primary_segment );
            
            foreach( $fragments[ 'secondary' ][ $i ] as $j => $secondary_segment )
            {
                $fragments[ 'tertiary' ][ $i ][ $j ] = explode( $separator_tertiary, $secondary_segment );
                
                //      There can only be 1 exception here : credentials with '@'
                //      -> simply append as extra fragment (in same level)
                foreach( $fragments[ 'tertiary' ][ $i ][ $j ] as $k => $test_credentials )
                {
                    $at = explode( $separator_credentials, $test_credentials );
                    
                    if ( count( $at ) > 1 )
                    {
                        $fragments[ 'tertiary' ][ $i ][ $j ][ $k ] = trim( $at[ 0 ]);
                        
                        $port = explode( $separator_port, $at[ 1 ]);
                        
                        if ( count( $port ) > 1 )
                        {
                            $fragments[ 'tertiary' ][ $i ][ $j ][ $k + 1 ] = trim( $port[ 1 ]);
                            $fragments[ 'tertiary' ][ $i ][ $j ][ $k + 2 ] = trim( $port[ 1 ]);
                        }
                        else {
                            $fragments[ 'tertiary' ][ $i ][ $j ][ $k + 1 ] = trim( $at[ 1 ]);
                        }
                    }
                }
            
                //      Collapse single childs, because we won't use the extra depth
                if ( count( $fragments[ 'tertiary' ][ $i ][ $j ]) == 1 ) {
                    $fragments[ 'tertiary' ][ $i ][ $j ] = array_pop( $fragments[ 'tertiary' ][ $i ][ $j ]);
                }
            }
            
            //      Collapse single childs, because we won't use the extra depth
            if ( count( $fragments[ 'tertiary' ][ $i ]) == 1 ) {
                $fragments[ 'tertiary' ][ $i ] = array_pop( $fragments[ 'tertiary' ][ $i ]);
            }
        }
        
        return $fragments[ 'tertiary' ];
    }
    
    return $fragments;
}


