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
 *  (Recursive)
 */
function list_files( $path, $options = array())
{
    //      Overridable defaults
    $options += array(
        'order' => RecursiveIteratorIterator::CHILD_FIRST,
        
        'mask' => '',//                         Regex to filter filenames &/or folder names
        'exclusions' => array( '.', '..' ),
        'tree' => false,//                      Recursive array reproducing "real" folder structures
        'max_items' => 9999,//                  Overload safety limit
        
        //      Note : the following params are not active when using 'tree'
        'max_depth' => 99,//                    By default, will look into up to 99 nested folders
        'files_only' => true,
        'dirs_only' => false,
        'sort_callback' => 'sort_files_list',// Sorts by filename by default, but allow to specify custom sorting callback for uasort()
        
        //      Cache
        'cache' => true,
        
        //      debug
        'warn_if_path_not_found' => false,
        
        //      Safety : prevents listing outside of project dir
        'limit_to_project_dir' => true,
    );
    extract( $options );
    
    $item_count = 0;
    $files_arr = array();
    
    
    //      Check file cache
    //      (only when $path is not an array)
    $cache_id = '';
    if ( $cache && !empty( $path ) && !is_array( $path ))
    {
        $unique_id = $path .','. $mask;
        $files_only ? $unique_id .= ',files_only' : '';
        $dirs_only ? $unique_id .= ',dirs_only' : '';
        $exclusions_diff = array_diff( $exclusions, array( '.', '..' ));
        !empty( $exclusions_diff ) ? $unique_id .= ',exclude['. join( ',', $exclusions_diff ) .']' : '';
        
        $files_arr = filecache_get( $unique_id );
        
        if ( isset( $files_arr )) {
            return $files_arr;
        }
    }
    
    
    //      Support mutliple paths
    if ( is_array( $path ))
        foreach( $path as $p )
            $files_arr = array_append_values( $files_arr, list_files( $p, $options ));
    
    //      Normal, single-path process
    else if ( !empty( $path ))
    {
        //      Lookup dir not found
        if ( !file_exists( $path )) {
            return array();
        }
        
        //      Prevents listing outside of project dir
        if ( $limit_to_project_dir )
        {
            $project_realpath = realpath( dirname(__FILE__) .'/../../' );
            $requested_realpath = realpath( $path );
            
            if ( strpos( $requested_realpath, $project_realpath ) !== 0 ) {
                return array();
            }
        }
        
        
        //      Begin file and/or folder search
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
                    $file_data = new stdClass();
                    $file_data->filename = $filename;
                    try
                    {
                        //$file_data->extension = $splFileInfo->getExtension(); //  PHP version >= 5.3.6
                        $file_data->path = $splFileInfo->getPath();
                        $file_data->fullpath = $file_data->path .'/'. $file_data->filename;
                        
                        //      Folder is the relative path of this element from the "base" path
                        //      Handle starting slash
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
                else {
                    $files_arr[] = end( $current );
                }
                
                //      Increment for testing max nb of items
                $item_count++;
            }
        }
        
        //      Save cache
        if ( $cache && !empty( $unique_id )) {
            filecache_set( $unique_id, $files_arr );
        }
    }
    
    //      Sorting
    if ( !empty( $files_arr ) && function_exists( $sort_callback )) {
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


