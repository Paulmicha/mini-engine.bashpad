<?php

/**
 *  @file
 *  Form Theming - Mutigroup = Repeatable Field groups
 *  
 *  (hacky, quick'n'dirty)
 */

//      We are in global scope #Gniii
$footer_extra_js .= '<script src="'. $base_path .'engine/core/assets/form_multigroup.js"></script>';


/**
 *  Repeatable field groups
 *  @return String : rendered HTML
 */
function multigroup( $label, $fields, $options = array())
{
    //      Overridable defaults
    $options += array(
        'label_attributes' => array(
            'class' => 'control-label',
        ),
        'group_namespace' => '',
    );
    extract( $options );
    
    //      Need a namespace for grouped repeatable fields
    if ( empty( $group_namespace ))
        $group_namespace = str_clean( $label );
    
    //      Polymorphism for param "$fields" :
    //          if String, we assume it's already well-formatted HTML.
    //          if Array, we build each element according to the following structure :
    //              array(
    //                  'func' => 'input_text',
    //                  'args' => array( "mon champ 1" ),
    //              )
    if ( is_string( $fields ))
        $repeatable_fields_html = $fields;
    else if ( is_array( $fields ))
    {
        $repeatable_fields_html = '';
        foreach( $fields as $field )
        {
            //      <Wrap field>
            //$repeatable_fields_html .= "<div class='control-group'>";
            
            //      Also support raw HTML string
            if ( is_string( $field ))
                $repeatable_fields_html .= $field;
            
            //      Callback function to make element
            else if ( !empty( $field[ 'func' ]) && function_exists( $field[ 'func' ]))
            {
                //      @evol 2013/07/14 01:50:40 - how to call functions with dynamically fetched parameters from an array ?
                if ( !empty( $field[ 'args' ]))
                {
                    //      Meanwhile, let's move along and support up to 4 args
                    $nb_args = count( $field[ 'args' ]);
                    switch( $nb_args )
                    {
                        case 4:
                            $arg4 = $field[ 'args' ][ 3 ];
                            $arg3 = $field[ 'args' ][ 2 ];
                            $arg2 = $field[ 'args' ][ 1 ];
                            $arg1 = $field[ 'args' ][ 0 ];
                            $repeatable_fields_html .= $field[ 'func' ]( $arg1, $arg2, $arg3, $arg4 );
                            break;
                        case 3:
                            $arg3 = $field[ 'args' ][ 2 ];
                            $arg2 = $field[ 'args' ][ 1 ];
                            $arg1 = $field[ 'args' ][ 0 ];
                            $repeatable_fields_html .= $field[ 'func' ]( $arg1, $arg2, $arg3 );
                            break;
                        case 2:
                            $arg2 = $field[ 'args' ][ 1 ];
                            $arg1 = $field[ 'args' ][ 0 ];
                            $repeatable_fields_html .= $field[ 'func' ]( $arg1, $arg2 );
                            break;
                        case 1:
                            $arg1 = $field[ 'args' ][ 0 ];
                            $repeatable_fields_html .= $field[ 'func' ]( $arg1 );
                            break;
                        default:
                            $repeatable_fields_html .= $field[ 'func' ]();
                            break;
                    }
                }
            }
            
            //      </Wrap field>
            //$repeatable_fields_html .= "</div>";
        }
        $repeatable_fields_html .= '<p class="text-center">';
        $repeatable_fields_html .= '<a class="repeatable-field-remove btn btn-small btn-danger" href="javascript:void(0)" style="display:none;"><i class="icon-remove"></i> Remove</a>';
        $repeatable_fields_html .= '</p>';
        
        //      Javascript makes use of an extra wrapper to work
        $repeatable_fields_html = "<div class='field-group-inner-wrap'>$repeatable_fields_html</div>";
    }
    
    //      Post-processing $repeatable_fields_html
    //      Just making sure all "names" attributes are "array-ified", for handling the submission of multiplied child fields
    //      @requires ../vendor/simple_html_dom.php
    $post_process_dom = str_get_html( $repeatable_fields_html );
    $tag_types_to_check = array( 'input', 'textarea', 'select' );
    foreach( $tag_types_to_check as $tag )
    {
        foreach( $post_process_dom->find( $tag ) as $i => $e )
        {
            //      Unique ids
            if ( !empty( $e->id ))
                $e->id .= '--multigroup-0';
            
            //      Name collisions
            if ( !empty( $e->name ))
            {
                //      Simple case : the "name" attribute was just a string
                //      -> we only need to add '[]' at the end to handle submission
                if ( strpos( $e->name, '[]' ) === false )
                {
                    //      @see notes below :
                    //      We still need to check for radios, because we won't be able to select
                    //      radios from the next multigroups without de-activating the ones from the first.
                    //if ( !empty( $e->type ) && $e->type == 'radio' )
                        $e->name .= '[multigroup-0]';
                    //else
                        //$e->name .= '[]';
                }
                
                //      The case where we already have '[]' in the "name" attribute (checkboxes)
                //      -> a namespace is needed to handle submission
                //      ex: "checkboxes-group[]" -> "checkboxes-group[multigroup-0][]"
                //      Note : javascript handles the numbering of this added "namespace"
                //      @see /engine/core/js/repeatable_form_fields.js
                else
                    $e->name = str_replace( '[]', '[multigroup-0][]', $e->name );
                
                //      Note : to reconstitute edit forms using multigroups, we must be able 
                //      to identify fields as part of a multigroup with their "name"
                //      @see form_set_values()
                //      -> let's use a prefix : $group_namespace (which must be unique)
                $e->name = 'multifield-'. $group_namespace .'-'. $e->name;
            }
        }
    }
    
    //      Idem for attribute 'for' in <label>
    foreach( $post_process_dom->find( 'label' ) as $i => $e )
        if ( !empty( $e->{'for'} ))
            $e->{'for'} .= '--multigroup-0';
    
    //      "Write" HTML post-process modifications
    $repeatable_fields_html = $post_process_dom;
    
    //      Chosen : javascript uses cloning to add new element, but :
    //      when chosen has already replaced some selects, can't just clone it, it doesnt work
    //      -> need to store the "unaltered" html
    $namespace = str_clean( $label, '_' );
    $repeatable_fields_html .= '
    <script type="text/javascript">
        if ( !repeatable_fields_html )
            var repeatable_fields_html = {};
        repeatable_fields_html.'. $namespace .' = "'. addcslashes( $repeatable_fields_html, '"' ) .'";
    </script>';
    
    //      "Add" button looks better inside label
    $label .= '<br /><a class="repeatable-field-add btn btn-small" href="javascript:void(0)"><i class="icon-plus"></i> Add</a>';
    
    return '
        <div class="control-group repeatable-wrap" data-namespace="'. $namespace .'">
            <label'. attributes( $label_attributes ) .'>'. $label .'</label>
            <div class="controls repeatable-fields-list">
                '. $repeatable_fields_html .'
            </div>
        </div>';
}


