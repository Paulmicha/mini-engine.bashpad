<?php

/**
 *  @file
 *  Launch any queries inside files named "install.sql" or "uninstall.sql",
 *  located inside dirs '/engine/core' and '/context/sql'
 *
 *  Install mini-engine core SQL tables
 *  For utilities like cache, log, variables, etc.
 *  
 *  @path /system/setup/install
 *  @path /system/setup/uninstall
 *  @path /system/setup/load-configuration
 */

//      Same script to handle all sorts of operations
$setup_operation = check_plain( arg( 2 ));
$head_page_title = "Setup operations";

//      Whitelist of available operations
$operations_whitelist = array(
    'install',
    'uninstall',
    'load-configuration',
);

if ( in_array( $setup_operation, $operations_whitelist ))
{
    
    
    //------------------------------------------------------------------------------------------------------------------------------------
    //      Confirmation
    
    
    if ( empty( $_GET[ 'yes' ]) && empty( $_POST[ 'poormans-submit-flag' ]))
    {
        //      Description
        switch( $setup_operation )
        {
            case 'install' :
                $head_page_title = "<i class='icon-download-alt'></i> Installation";
                $content .= "<p>Queries inside files named <code>install.sql</code> in <code>config/schema</code> will be executed. </p>";
                $content .= "<p><i class='icon-long-arrow-right icon-large'></i> Core SQL Tables will be created (if they don't exist yet).</p>";
                $content .= "<p>";
                $content .= l( "<i class='icon-download-alt'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-primary' )));
                $content .= "</p>";
                break;
            
            case 'uninstall' :
                $head_page_title = "<i class='icon-trash'></i> Drop DB tables";
                $content .= "<p>Queries inside files named <code>uninstall.sql</code> in <code>config/schema</code> will be executed. </p>";
                $content .= "<p><i class='icon-long-arrow-right icon-large'></i> Core SQL Tables will all be <strong>DROPPED</strong>.</p>";
                $content .= "<p>";
                $content .= l( "<i class='icon-trash'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-danger' )));
                $content .= "</p>";
                break;
            
            case 'load-configuration' :
                
                //      @evol 2013/12/18 23:11:56 - I'd like to take the time for a proper import / export feature :
                //          - would require write access in 'config/presets/custom'
                //          - path naming convention : 'config/presets/custom/2013/12/18/001/ENTITY_TYPE.php' (for example)
                //          - [in progress] streamline the config load process with GUIDs
                //          - [in progress] display a form to select which config to restore
                //          - [planned] mini ORM ("natural" consequence of GUIDs + exportability)
                
                $head_page_title = "<i class='icon-upload'></i> Load configuration from files";
                
                $content .= "<p>The following <code>*.inc</code> files in <code>config/presets</code> can be included.<br>They should handle operations such as loading some default configuration into the database.</p>";
                
                $content .= "<p>Note : all these custom operations must not produce duplicates, so that they can be launched again without compromizing existing data.
                    <br>They will most likely overwrite any value different than what's in DB.</p>";
                
                $content .= "<h3>Files in <code>default</code> folder should be imported</h3>";
                $content .= "<p><i class='icon-long-arrow-right icon-large'></i> Core default <strong>tags</strong> and <strong>commands</strong> will be created.</p>";
                
                //      Select '.inc' files to load configuration from
                $content .= render_form( file_selector( 'config/presets', array( 'mask' => '`.\.inc`i' )));
                
                //$content .= "<p>";
                //$content .= l( "<i class='icon-upload'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-primary' )));
                //$content .= "</p>";
                
                break;
        }
    }
    
    
    
    //------------------------------------------------------------------------------------------------------------------------------------
    //      Execution
    
    
    else
    {
        switch( $setup_operation )
        {
            case 'load-configuration' :
                
                $head_page_title = "<i class='icon-upload'></i> Loading configuration";
                
                if ( empty( $_POST[ 'files' ])) {
                    $content .= alert( "No configuration files were selected", 'warning' );
                }
                else
                {
                    foreach( $_POST[ 'files' ] as $serialized_file_data )
                    {
                        $f = safe_unserialize( $serialized_file_data );
                        
                        //      Feedback
                        $content .= "<hr>";
                        $content .= "<p>Processing <code>". $f->path .'/'. $f->filename ."</code> ...</p>";
                        include $f->path .'/'. $f->filename;
                    }
                }
                
                break;
            
            
            case 'install' :
            case 'uninstall' :
                
                $head_page_title = ucfirst( $setup_operation ) ." SQL tables";
                
                //      Load all SQL files that are named like "$setup_operation.sql"
                $files_arr = list_files( 'config/schema', array( 'mask' => "`^$setup_operation\.sql`i" ));
                
                if ( !empty( $files_arr ))
                {
                    foreach( $files_arr as $f )
                    {
                        $sql_file = $f->path .'/'. $f->filename;
                        
                        if ( !file_exists( $sql_file ))
                            $content .= alert( "<strong>Erreur</strong> : fichier inexistant ~ <code>". check_plain( print_r( $sql_file, 1 )) ."</code>", 'error' );
                        else
                        {
                            $content .= alert( "Processing file '$sql_file'..." );
                            
                            $nb = 0;
                            $sql = file_get_contents( $sql_file );
                            $queries_arr = explode( ';', $sql );
                            
                            foreach( $queries_arr as $i => $query )
                            {
                                $empty_query_check = str_clean( $query );
                                if ( !empty( $empty_query_check ))
                                {
                                    if ( !db_query( $query ))
                                        $content .= alert( "<strong>Erreur</strong> : failed query ~ <code>". check_plain( print_r( $query, 1 )) ."</code>", 'error' );
                                    else
                                        $nb++;
                                }
                            }
                            
                            if ( $nb )
                                $content .= alert( "$nb queries successfully executed.", 'success' );
                        }
                    }
                }
                else
                    $content .= alert( "No sql files were found", 'warning' );
                
                break;
        }
    }
}
else
    $content .= alert( "(Nothing to do here)" );





