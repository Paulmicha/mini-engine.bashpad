<?php

/**
 *  @file
 *  Configuration - load
 *  
 *  @path /system/setup/config/load
 */


//      Confirmation
if ( empty( $_GET[ 'yes' ]) && empty( $_POST[ 'poormans-submit-flag' ]))
{
    $head_page_title = "<i class='icon-download-alt'></i> Load configuration from files";
    
    //      Using entities
    $content .= "<p>Any <code>*.inc</code> files in <code>config/import</code> or <code>config/export</code> can be included (here, inclusion = execution).<br/>
        They should handle operations such as loading some default configuration into the database.</p>
        <p>Note : Any custom operations should not produce duplicates, so that they can be launched again without compromizing existing data
        (e.g. to restore some defaults by overriding current config)</p>";
    
    $content .= "<p><i class='icon-long-arrow-right icon-large'></i> Core default <strong>tags</strong> and <strong>commands</strong> will be created.</p>";
    
    $content .= render_form( file_selector( array( 'config/import', 'config/export' ), array(
        'mask' => '`.\.inc`i',
        'checked' => 'checked',
        'title' => "<i class='icon-long-arrow-right icon-large'></i> Default <strong>tags</strong> and <strong>commands</strong> should be created</small>",
    )));
}

//      Execution
else
{
    foreach( $_POST[ 'files' ] as $serialized_file_data )
    {
        $f = safe_unserialize( $serialized_file_data );
        $entity_name = str_replace( '.inc', '', $f->filename );
        
        //      Feedback
        $content .= "<hr>";
        $content .= "<p>Processing <strong>$entity_name</strong> - <code>". $f->fullpath ."</code> ...</p>";
        
        include $f->fullpath;
    }
}


