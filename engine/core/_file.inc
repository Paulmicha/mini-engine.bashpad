<?php

/**
 *  @file
 *  File utilities
 */


/**
 *  File size helper
 *  @see http://www.php.net/manual/fr/function.filesize.php#106569
 */
function human_filesize( $filepath, $decimals = 2 )
{
    $sz = 'BKMGTPEZY';
    $bytes = filesize( $filepath );
    $factor = floor(( strlen( $bytes ) - 1 ) / 3 );
    return sprintf( "%.{$decimals}f", $bytes / pow( 1024, $factor )) . @$sz[ $factor ];
}



/**
 *  Build dirs & files tree
 *  @see http://php.net/manual/fr/class.recursiveiteratoriterator.php
 */
function list_files( $path, $options = array())
{
    //      Overridable defaults
    $options += array(
        //'order' => RecursiveIteratorIterator::LEAVES_ONLY, //  (default)
        //'order' => RecursiveIteratorIterator::SELF_FIRST,
        'order' => RecursiveIteratorIterator::CHILD_FIRST,
        'mask' => '',
        'exclusions' => array( '.', '..' ),
        'tree' => false,
        'max_items' => 9999,
        
        //      Note : the following params are not effective when using 'tree'
        'max_depth' => 999,
        'files_only' => true,
        'dirs_only' => false,
        'sort_callback' => 'sort_files_list',
        
        //      debug
        'warn_if_path_not_found' => false,
    );
    extract( $options );
    
    $files_arr = array();
    $item_count = 0;
    
    //      Support mutliple paths
    if ( is_array( $path ))
        foreach( $path as $p )
            $files_arr = array_append_values( $files_arr, list_files( $p, $options ));
    
    //      Normal, single-path process
    else if ( !empty( $path ))
    {
        if ( !file_exists( $path ))
        {
            if ( $warn_if_path_not_found )
            {
                if ( function_exists( 'alert' )) {
                    return alert( "Directory <code>$path</code> does not exist, or is inaccessible.", 'warning' );
                }
                return "Directory <code>$path</code> does not exist, or is inaccessible.";
            }
            
            return array();
        }
         
        $ritit = new RecursiveIteratorIterator( new RecursiveDirectoryIterator( $path ), $order );
        foreach ( $ritit as $splFileInfo )
        {
            //      Limit max nb of items browsed
            if ( $max_items > 0 && $item_count >= $max_items )
                break;
            
            $current = array();
            $is_dir = $splFileInfo->isDir();
            $filename = $splFileInfo->getFilename();
            if ( $tree && $is_dir && !in_array( $filename, $exclusions, true ) && $ritit->getDepth() <= $max_depth )
                $current = array( $filename => array());
            else
            {
                //      Unix-like names exclusions,
                //      Apply filter on filename (if provided),
                //      Filters by type (dirs/file) -- if possible, cf. 'tree' option
                if (( in_array( $filename, $exclusions, true )) ||
                    ( $ritit->getDepth() > $max_depth ) ||
                    ( !empty( $mask ) && !preg_match( $mask, $filename )) ||
                    (( $files_only && $is_dir ) || ( $dirs_only && !$is_dir )))
                    continue;
                else
                {
                    //$current = array( $filename );
                    
                    //      update 2013/03/21 15:44:57 - get more data for files
                    $file_data = new stdClass();
                    $file_data->filename = $filename;
                    try
                    {
                        //$file_data->extension = $splFileInfo->getExtension(); //  PHP version >= 5.3.6
                        $file_data->path = $splFileInfo->getPath();
                        $file_data->fullpath = $file_data->path .'/'. $file_data->filename;
                        
                        //      Folder is the relative path of this element from the "base" path
                        //      Handle starting slash
                        //$file_data->folder = substr( str_replace( $path, '', $file_data->path ), 1 );
                        $file_data->folder = str_replace( $path, '', $file_data->path );
                        if ( substr( $file_data->folder, 0, 2 ) == "./" )
                            $file_data->folder = substr( $file_data->folder, 2 );
                        else if ( substr( $file_data->folder, 0, 1 ) == "/" )
                            $file_data->folder = substr( $file_data->folder, 1 );
                        
                        $file_data->realpath = $splFileInfo->getRealPath();
                        $file_data->size = human_filesize( $file_data->fullpath );
                        $file_data->bsize = $splFileInfo->getSize();
                        $file_data->depth = $ritit->getDepth();
                    }
                    catch ( Exception $e ) {}
                    
                    //      Using 'tree' allows us to use the filename as key
                    //      (no risk of collision in the same folder)
                    if ( $tree )
                        $current = array( $filename => $file_data );
                    else
                        $current = array( $file_data );
                }
            }
            
            if ( !empty( $current ))
            {
                if ( $tree )
                {
                    //      Recursively merge the tree
                    for ( $depth = $ritit->getDepth() - 1; $depth >= 0; $depth-- ) {
                        $current = array( $ritit->getSubIterator( $depth )->current()->getFilename() => $current );
                    }
                    $files_arr = array_merge_recursive( $files_arr, $current );
                    
                }
                else
                    $files_arr[] = end( $current );
                    //$files_arr[ $filename ] = end( $current );
                
                //      Increment for testing max nb of items
                $item_count++;
            }
        }
    }
    
    //      Sorting
    if ( function_exists( $sort_callback )) {
        uasort( $files_arr, $sort_callback );
    }
    
    return $files_arr;
}



/**
 *  Array sort callback
 *  (by filename)
 */
function sort_files_list( $a, $b ) {
    return ( $a->filename < $b->filename ) ? -1 : 1;
}


