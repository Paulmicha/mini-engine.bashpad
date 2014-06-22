<?php

/**
 *  @file
 *  Form Theming helpers - file selector
 */


/**
 *  Simple file selector
 */
function file_selector( $path, $options = array())
{
    $html = '';
    
    //      Overridable defaults
    //      @see list_files() in engine/core/helpers_file.inc
    $options += array(
        'title' => '',
        'tree' => true,
        'multiple' => true,
        'checked' => false,
    );
    extract( $options );
    
    if ( empty( $title )) {
        $title = 'Select file'. ( $multiple ? 's' : '' );
    }
    
    //      Display folder title when inputting several folders
    if ( is_array( $path ))
    {
        foreach ( $path as $p )
        {
            $files = list_files( $p, $options );
            $html .= "<strong>$p&nbsp;<i class='icon-folder-open'></i></strong>";
            $html .= '<div style="padding-left:1em">';
            $html .= file_selector_recursive_builder( $files, $multiple, $checked );
            $html .= '</div>';
        }
        
        $html = fieldset( $title, $html );
    }
    else
    {
        $files = list_files( $path, $options );
        $html .= fieldset( $title, file_selector_recursive_builder( $files, $checked, $multiple ));
    }
    
    return $html;
}



/**
 *  Simple file selector
 *  Recursive helper
 */
function file_selector_recursive_builder( $files, $multiple, $checked = false, $parent_dirname = '' )
{
    $html = '';
    
    if ( !empty( $files ))
    {
        //      It's a file
        if ( is_object( $files ))
        {
            $attr = array(
                'name' => 'files[]',
                'value' => safe_serialize( $files ),
                'checked' => $checked,
            );
            
            if ( $multiple ) {
                $html .= input_checkbox( $files->filename, $attr );
            }
            else {
                $html .= input_radio( $files->filename, $attr );
            }
        }
        
        //      It's a folder
        else if ( is_array( $files ))
        {
            foreach( $files as $dir_name => $dir_content_arr )
            {
                if ( !empty( $dir_content_arr ))
                {
                    $html .= '<li>';
                    
                    //      If child isn't array, don't show label
                    if ( is_array( $dir_content_arr ))
                    {
                        $html .= '<strong>';
                        
                        if ( !empty( $parent_dirname )) {
                            $dir_name = "$parent_dirname/$dir_name";
                        }
                        
                        $html .= $dir_name;
                        $html .= '&nbsp;<i class="icon-folder-open"></i>';
                        $html .= '</strong>';
                    }
                    
                    $html .= file_selector_recursive_builder( $dir_content_arr, $multiple, $checked, $dir_name );
                    
                    $html .= '</li>';
                }
            }
            
            if ( !empty( $html ))
            {
                //      CSS Class for level 0
                if ( empty( $parent_dirname )) {
                    $html = "<ul class='file-selector-tree'>$html</ul>";
                }
                else {
                    $html = "<ul>$html</ul>";
                }
            }
        }
    }
    
    return $html;
}


