<?php

/**
 *  @file
 *  Default commands
 */

//      Set default commands
//      Fetch all php files in 'engine/core/config/bash'
//      They must contain a single variable $command containing an array of bash commands data
$files_arr = list_files( 'engine/core/config/bash', array( 'mask' => "`.+\.php`i" ));
if ( !empty( $files_arr ))
{
    foreach( $files_arr as $f )
    {
        //      Feedback
        $content .= "<br>Loading command defined in <code>". $f->path .'/'.$f->filename ."</code> ...";
        include $f->path .'/'.$f->filename;
        
        //      Default creation datestamp
        $command[ 'datestamp' ] = datestamp();
        
        //      Serialize data
        $command[ 'data' ] = safe_serialize( $command[ 'data' ]);
        
        //      Prepare tags (if any)
        //      -> need to unset the key 'tags' for DB insertion,
        //      so we make a 'copy' in $tags_arr for later
        $tags_arr = array();
        if ( !empty( $command[ 'tags' ]))
        {
            $tags_arr = $command[ 'tags' ];
            unset( $command[ 'tags' ]);
        }
        
        //      Error checking
        if ( !empty( $command ))
        {
            $id_command = insert_if_no_existing_record( 'command', $command, array( 'unique_field_name' => 'title' ));
            
            //      Feedback
            if ( !empty( $id_command ))
            {
                $content .= alert( "Command '". $command[ 'title' ] ."' successfully processed.", 'success' );
                
                //      Set tags
                if ( !empty( $tags_arr ))
                {
                    $preprocessed_tags = array();
                    foreach( $tags_arr as $tag_title )
                    {
                        $id_tag = get_tag_by_title( $tag_title );
                        if ( !empty( $id_tag ))
                            $preprocessed_tags[ 'command' ][ $id_command ][ 'tags' ][] = $id_tag;
                        else
                            $preprocessed_tags[ 'command' ][ $id_command ][ 'new_tags' ][] = $tag_title;
                    }
                    
                    //      Save tags associations
                    set_tags( $preprocessed_tags );
                }
            }
            else
                $content .= alert( "Error : could not insert command. That's all I know.", 'error' );
        }
        else
            $content .= alert( "This file does not seem to define a valid command.", 'warning' );
    }
}
else
    $content .= alert( "No commands files were found in <code>engine/core/config/bash</code> -> cannot generate default commands.", 'warning' );

//      Feedback
$content .= "<hr>";
$content .= alert( "Default commands processed.", 'success' );


