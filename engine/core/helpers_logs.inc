<?php

/**
 *  @file
 *  Mini-engine helpers
 *  Logging with files
 */


/**
 *  Write & purge logs
 *  par défaut purge tous les fichiers vieux de plus de 7 jours
 */
function custom_app_log( $str, $log_type = 'app', $options = array())
{
    if ( !empty( $str ))
    {
        //      Overridable defaults
        $options += array(
            'datestamp' => date( "Ymd" ),
            'timestamp' => time(),
            'purge_limit' => 7 * 24 * 60 * 60,
            'purge' => true,
            'path' => "core/writeable/logs",
            'char_linebreak' => "\n",
        );
        extract( $options );
        
        //      Auto purge old files
        //      @todo 2013/02/28 17:01:32 - perf opti !!!
        if ( $purge ) {
            purge_logs( $options );
        }
        
        //      Write log
        $str = date( "Y-m-d G:i:s" ) ." : ". $str.$char_linebreak;
        $expected_filename = "log-$datestamp-$log_type.txt";
        //$log_files_arr = file_scan_directory( $path, '`\.txt`i' );
        $log_files_arr = list_files( $path, array( 'mask' => '`\.txt`i' ));
        
        //      Check first if it exists
        if ( $log_files_arr )
        {
            foreach( $log_files_arr as $file )
            {
                if ( $file->basename == $expected_filename )
                {
                    //      Éviter les gros fichiers de logs
                    $i = 0;
                    $fragmentation_filename = $file->filename;
                    while( filesize( $fragmentation_filename ) > 300 * 1024 )
                    {
                        $i++;
                        $fragmentation_filename = "$path/log-$datestamp-$log_type-$i.txt";
                    }
                    
                    //      Écrire à la suite du fichier
                    return file_put_contents( $fragmentation_filename, $str, FILE_APPEND );
                }
            }
        }
        
        //      Si pas de fichier existant, il est créé
        return file_put_contents( "$path/$expected_filename", $str ); 
        
        //      debug
        //      format $log_files_arr :
        //Array
        //(
        //    [path/to/file.txt] => stdClass Object
        //    (
        //        [filename] => path/to/file.txt
        //        [basename] => file.txt
        //        [name] => file
        //    )
        //)
    }
    return false;
}



/**
 *  Purge log files
 */
function purge_logs( $options = array())
{
    //      Overridable defaults
    $options += array(
        'datestamp' => date( "Ymd" ),
        'timestamp' => time(),
        'path' => "logs",
        'purge_limit' => 1296000
    );
    extract( $options );
    
    $log_files_arr = file_scan_directory( $path, '.txt' );
    if ( $log_files_arr )
    {
        $errors = array();
        foreach( $log_files_arr as $file )
        {
            $file_age_timestamp = filemtime( $file->filename );
            if ( $file_age_timestamp < $timestamp + $purge_limit )
            {
                if ( !unlink( $file->filename )) {
                    $errors[] = $file;
                }
            }
        }
    }
    
    //      Feedback
    if ( !empty( $errors )) return $errors;
    else return true;
}


