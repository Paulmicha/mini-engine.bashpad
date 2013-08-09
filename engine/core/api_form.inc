<?php

/**
 *  @file
 *  Form API
 *  
 *  Beware : here be dragons.
 */

//      We are in global scope #Gniii
$footer_extra_js .= '<script src="'. $base_path .'engine/core/js/repeatable_form_fields.js"></script>';


/**
 *  Basic form rendering
 *  Handles submit with callback
 */
function render_form( $form_html_content, $submit_handler = null, $options = array())
{
    //      Overridable defaults
    $options += array(
        'submit_handler_params' => array(),
        'form_attributes' => array(),
        
        //      Keep those in comments as a reminder
        //'validation_handler' => null,
        //'destination' => null,
        
        //      Setting this to 'return callback result' means :
        //      Get the callback return value from render_form()
        'when_posted' => 'return callback result',
    );
    
    //      Handling defaults params without losing defaults (recursive merging issue)
    $options[ 'submit_handler_params' ] += array(
        
        //      This allows to rebuild the form easily in case of an error
        'form_html' => form( $form_html_content, $options[ 'form_attributes' ]),
    );
    
    extract( $options );
    
    //      Is it posted already ?
    //      @evol 2013/07/11 00:17:52 - Permettre de gérer plusieurs fomulaires par page avec chacun leur "submit flag"
    if ( !empty( $_POST[ 'poormans-submit-flag' ]) && !empty( $submit_handler ) && function_exists( $submit_handler ))
    {
        //      Validation callback
        if ( !empty( $validation_handler ) && function_exists( $validation_handler ))
        {
            $validation_data = $validation_handler( $submit_handler_params );
            
            //      Allow validation callback to return "sanitized" inputs
            if ( is_array( $validation_data[ 'sanitized_values' ]))
                $submit_handler_params[ 'sanitized_values' ] = $validation_data[ 'sanitized_values' ] + $_POST;
            
            //      Speed up the work in submission handler :
            //      the key 'value', if set, means the ones we can directly save in the database
            //      @todo 2013/07/22 11:18:37 - remove this : using param $validation_data by reference
            //      @see _environment_form_validate()
            if ( !empty( $validation_data[ 'values' ]) && empty( $submit_handler_params[ 'values' ]))
                $submit_handler_params[ 'values' ] = $validation_data[ 'values' ];
            
            //      Error handling : display the form
            if ( !empty( $validation_data[ 'errors' ]))
            {
                $return = '';
                $return .= alert( "Error(s) occured during form validation", 'warning' );
                
                //      Errors can be arrays or strings
                if ( is_string( $validation_data[ 'errors' ]))
                    $return .= $validation_data[ 'errors' ];
                else if ( is_array( $validation_data[ 'errors' ]))
                    foreach( $validation_data[ 'errors' ] as $err )
                        $return .= alert( $err, 'error' );
                
                $return .= "<hr>";
                
                //      Need to display the form with preset values
                //      @todo 2013/07/15 21:48:01 - for now this only works for string values
                $return .= form_set_values( $submit_handler_params[ 'form_html' ], $submit_handler_params[ 'sanitized_values' ]);
                
                return $return;
            }
        }
        
        //      Asked for callback return value
        if ( $when_posted == 'return callback result' )
            return $submit_handler( $submit_handler_params );
        
        //      Allow just calling it
        $submit_handler( $submit_handler_params );
        
        //      Redirect (untested @todo 2013/07/10 19:57:04)
        if ( !empty( $destination ))
            header( "Location: $destination" );
    }
    
    //      Just return rendered form HTML if it's not submitted
    return form( $form_html_content, $form_attributes );
}



/**
 *  Form helper : set values in rendered html
 *  @return String : rendered form HTML
 */
function form_set_values( $form_html_content, $values = array())
{
    //      This is ugly™, but it's quicker
    //      -> parsing DOM of rendered HTML to fill-in the values
    $post_process_dom = str_get_html( $form_html_content );
    
    //      debug
    //krumo( $values );
    
    //      Handle multigroups 'clones'
    if ( !empty( $values[ 'multigroups' ]))
    {
        foreach( $values[ 'multigroups' ] as $multigroup_namespace => $multigroup_values )
        {
            //      debug
            //krumo( $multigroup_values );
            
            //      Failsafe
            if ( empty( $multigroup_values ))
                continue;
            
            //      Count how many we'll need to clone
            $any_field = reset( $multigroup_values );
            $nb_clones = count( $any_field );
            
            //      Determine unique numbers
            $numbers_list_arr = array();
            foreach( array_keys( $any_field ) as $mg_numbered_str )
                $numbers_list_arr[] = intval( str_replace( 'multigroup-', '', $mg_numbered_str ));
            
            //      Find and clone
            if ( $nb_clones > 1 )
            {
                //      Get HTML string to clone
                $first_multigroup_inner_wrap_dom = $post_process_dom->find( "div[data-namespace='$multigroup_namespace'] div.field-group-inner-wrap", 0 );
                
                //      Make HTML with repeated groups
                $fields_group_wrap_html = '';
                foreach( $numbers_list_arr as $nb )
                    $fields_group_wrap_html .= str_replace( 'multigroup-0', "multigroup-$nb", $first_multigroup_inner_wrap_dom->outertext );
                
                //      "Remove buttons" initial invisibility must be cancelled when there are several "entries" in multigroup
                $GLOBALS[ 'footer_extra_js' ] .= "<script type='text/javascript'>(function($)
{
    //      Override invisibility of remove buttons for multigroup '$multigroup_namespace'
    $( 'div[data-namespace=$multigroup_namespace].repeatable-wrap .repeatable-field-remove' ).css( 'display', 'inline' );
    $( '.repeatable-field-remove' ).bind( 'click', remove_handler );
    
})(jQuery);</script>";
                
                $first_multigroup_inner_wrap_dom->outertext = $fields_group_wrap_html;
                
                //      Write DOM modifications + Reload DOM object
                $form_html_content = $post_process_dom;
                $post_process_dom = str_get_html( $form_html_content );
            }
        }
    }
    
    //      From the values to the dom
    foreach( $values as $name => $value )
    {
        if ( is_array( $value ))
            form_values_recursive_filler( $post_process_dom, $value, $name );
        else
        {
            //      Textareas
            foreach( $post_process_dom->find( 'textarea' ) as $i => $e )
                if ( $e->name == $name )
                    $e->innertext = $value;
            
            //      Inputs (text, checkbox, radio, password, etc.)
            foreach( $post_process_dom->find( 'input' ) as $i => $e )
                if ( $e->name == $name )
                    if (( $e->type == 'checkbox' || $e->type == 'radio' ) && $e->value == $value )
                        $e->checked = 'checked';
                    else
                        $e->value = $value;
            
            //      Single Select
            foreach( $post_process_dom->find( 'select' ) as $i => $e )
                if ( $e->name == $name )
                    foreach( $e->find( 'option' ) as $j => $sub_e )
                        if ( $sub_e->value == $value )
                            $sub_e->selected = 'selected';
        }
    }
    
    //      Write DOM modifications
    $form_html_content = $post_process_dom;
    
    return $form_html_content;
}



/**
 *  Recursive attribute "filler"
 *  
 *  @param PHP Simple HTML DOM Parser $post_process_dom : instance in process
 *  @param Array $values : Values to fill the DOM elemnts with
 *  @param String $parent_name : base "name" attribute - e.g. in an attribute like 'the-name[group]',
 *      this param would be 'the-name'.
 *  
 *  @see form_set_values()
 */
function form_values_recursive_filler( &$post_process_dom, $values, $parent_name = '' )
{
    if ( is_array( $values ))
    {
        foreach( $values as $key => $value )
        {
            //      debug
            //print '<br><code>'. $parent_name .' > '. $key .'</code> = '. $value;
            
            //      Simple : we got strings
            if ( is_string( $value ))
            {
                
                //        Support "name" attribute values like 'the-name[]' or 'the-name[group]'
                $name_selector = $parent_name .'['. $key .']';
                if ( is_numeric( $key ))
                    $name_selector = $parent_name .'[]';
                
                //      debug
                //print '<br><code>'. $parent_name .' > '. $key .'</code> = '. $value;
                //print ' - <code>name="'. $name_selector .'"</code>';
                
                //      Match by name & set values
                foreach( $post_process_dom->find( '*[name="'. $name_selector .'"]' ) as $e )
                    simpledom_form_field_setter( $e, $value );
            }
            
            //      Arrays : multigroups or deeper field names
            else
            {
                //      Pruning first level of all multigroups
                if ( $key == 'multigroups' )
                    foreach( $value as $multigroup_namespace => $multigroup_values )
                        form_values_recursive_filler( $post_process_dom, $multigroup_values );
                
                //      Reproduce nested name attribute value
                else if ( strpos( $key, 'multigroup-' ) === 0 )
                    form_values_recursive_filler( $post_process_dom, $value, $parent_name ."[$key]" );
                
                //      'Normal' multi-value fields
                else
                    form_values_recursive_filler( $post_process_dom, $value, $key );
            }
        }
    }
}



/**
 *  Assign form fields value
 */
function simpledom_form_field_setter( &$e, $value_str )
{
    switch( $e->tag )
    {
        case 'textarea' :
            $e->innertext = $value_str;
            break;
        
        case 'input' :
            if (( $e->type == 'checkbox' || $e->type == 'radio' ) && $e->value == $value_str )
                $e->checked = 'checked';
            else
                $e->value = $value_str;
            break;
        
        case 'select' :
            foreach( $e->children() as $sub_e )
                if ( $sub_e->value == $value_str )
                    $sub_e->selected = 'selected';
            break;
    }
}



/**
 *  Recursive quick string Sanitization helper
 *  @param Array $posted : posted data (usually a copy of '$_POST')
 *  @param Array $exceptions : list of keys to leave untouched
 */
function form_recursive_sanitization( &$posted, $exceptions = array())
{
    //      By default, I'm leaving 'password' key as NOT to be sanitized
    $exceptions += array( 'password' );
    
    foreach( $posted as $key => $val )
        if ( !in_array( $key, $exceptions ) && is_string( $val ))
            $posted[ $key ] = check_plain( $val );
        else if ( is_array( $val ))
            form_recursive_sanitization( $posted[ $key ]);
}



/**
 *  Simple Form HTML wrapper
 *  
 *  @param String $content : HTML of form Content
 *  @return String : Form HTML
 */
function form( $content, $options = array())
{
    //      Overridable defaults
    $options += array(
        'attributes' => array(
            'class' => 'form-horizontal',
            'action' => get_current_url( array( 'absolute' => false )),
            'accept-charset' => "UTF-8",
            'role' => 'form',
            'content' => '',
            'id' => null,
            'method' => 'post',
        ),
        
        //      Use a poorman's submit flag
        'poormans_submit_flag' => true,
        
        //      in form_render(), we may need to have a say in this value
        //'same_request_form_count_override' => 0,
        
        //      Default action : submit button
        'actions' => input( array(
            'type' => 'submit',
            'value' => 'Go',
            'class' => 'btn btn-primary btn-large',
        )),
    );
    extract( $options );
    
    //      Permettre de gérer plusieurs fomulaires par page avec chacun leur "submit flag"
    //      @evol 2013/07/11 00:17:31 - pas encore géré avec render_form()
    if ( $poormans_submit_flag )
    {
        //      L'utilisation du "static" ne peut évidemment fonctionner que si tous les formulaires potentiellement en conflit
        //      sont "fabriqués" pendant la même "http request" (sinon il suffit d'opt-out 'poormans_submit_flag' dans les options)
        static $same_request_form_count;
        if ( !$same_request_form_count )
            $same_request_form_count = 1;
        
        //      @evol 2013/07/14 19:51:28 - pas si simple à automatiser, pas le temps là
        //      @see form_render()
        //if ( isset( $options[ 'same_request_form_count_override' ]))
        //    $same_request_form_count = $options[ 'same_request_form_count_override' ];
        
        $val = !empty( $attributes[ 'id' ]) ? $attributes[ 'id' ] : $same_request_form_count;
        $content .= '<input type="hidden" name="poormans-submit-flag['. $val .']" value="'. $val .'" />';
        
        $same_request_form_count++;
    }
    
    //      Form actions
    if ( !empty( $actions ))
        $content .= '<div class="form-actions">'. $actions .'</div>';
    
    $attributes = attributes( $attributes );
    return "<form$attributes>$content</form>";
}



/**
 *  Render input (generic helper)
 *  @return String : rendered html
 */
function input( $options = array())
{
    //      Overridable defaults
    $options += array(
        'type' => 'text',
        'placeholder' => '',
        'required' => '',
        //'name' => '',
        //'id' => '',
        //'value' => '',
        'class' => 'span5',
    );
    
    //      Required
    $required = $options[ 'required' ];
    unset( $options[ 'required' ]);
    
    //      Attributes shortcut
    $attributes = attributes( $options );
    if ( !empty( $required ))
        $attributes .= " required";
    
    return "<input$attributes />";
}



//--------------------------------------------------------------------------------------------------
//      Repeatable Field groups


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



//--------------------------------------------------------------------------------------------------
//      Select


/**
 *  Select multiple (helper wrapper function)
 *  @return String : rendered html
 */
function multiple_select( $label, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'select_attributes' => array(
            'multiple' => 'multiple',
        ),
    );
    return select( $label, $options_arr, $options );
}



/**
 *  Render text input with labe (control group)
 *  NB : requiert la classe "form-horizontal" sur la balise <form>
 *  @return String : rendered html
 */
function select( $label, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'select_attributes' => array(),
        'label_attributes' => array(
            'class' => 'control-label',
        ),
    );
    extract( $options );
    
    //      Fill-in optional defaults
    if ( empty( $select_attributes[ 'name' ]))
        $select_attributes[ 'name' ] = str_clean( $label );
    if ( empty( $select_attributes[ 'id' ]) && !empty( $select_attributes[ 'name' ]))
            $select_attributes[ 'id' ] = $select_attributes[ 'name' ];
    if ( empty( $label_attributes[ 'for' ]) && !empty( $select_attributes[ 'id' ]))
        $label_attributes[ 'for' ] = $select_attributes[ 'id' ];
    
    return '
        <div class="control-group">
            <label'. attributes( $label_attributes ) .'>'. $label .'</label>
            <div class="controls">
                '. select_raw( $label, $options_arr, $select_attributes ) .'
            </div>
        </div>';
}



/**
 *  Render text input
 *  @return String : rendered html
 */
function select_raw( $label, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'name' => '',
        'required' => '',
        //'id' => '',
        'class' => 'span5',
        //'help-inline' => "",
        //'help-block' => "",
        //'multiple' => 'multiple',
        'preserve_numeric_keys' => false,
        'default_value' => '',
    );
    
    //      Anticipate attributes shortcut
    $required = $options[ 'required' ];
    unset( $options[ 'required' ]);
    $preserve_numeric_keys = $options[ 'preserve_numeric_keys' ];
    unset( $options[ 'preserve_numeric_keys' ]);
    $default_value = $options[ 'default_value' ];
    unset( $options[ 'default_value' ]);
    
    //      Fill-in optional defaults
    if ( empty( $options[ 'name' ]))
        $options[ 'name' ] = str_clean( $label );
    if ( empty( $options[ 'id' ]) && !empty( $options[ 'name' ]))
        $options[ 'id' ] = $options[ 'name' ];
    
    //      Multiselects must have their 'name' with brackets
    //      Note : no need to worry about strpos() operator for this condition (it won't ever return 0)
    if ( $options[ 'multiple' ] == 'multiple' && !strpos( $options[ 'name' ], '[]' ))
        $options[ 'name' ] .= '[]';
    
    //      Options html
    $options_html = '';
    if ( !empty( $options_arr ) && is_array( $options_arr ))
    {
        foreach( $options_arr as $key => $val )
        {
            $option_label = '';
            $option_attr = array();
            
            if ( !is_numeric( $key ) || $preserve_numeric_keys )
                $option_attr[ 'value' ] = check_plain( $key );
            
            if ( is_array( $val ) && !empty( $val[ 'label' ]))
            {
                $option_label = $val[ 'label' ];
                unset( $val[ 'label' ]);
                
                if ( empty( $option_attr[ 'value' ]))
                    $option_attr = array( 'value' => str_clean( $option_label ));
                
                $option_attr = $val + $option_attr;
            }
            else
            {
                $option_label = $val;
                if ( empty( $option_attr[ 'value' ]))
                    $option_attr = array( 'value' => str_clean( $option_label ));
            }
            
            //      debug
            //krumo( $option_attr );
            
            //      Default value
            if ( !empty( $default_value ) && $default_value == $option_attr[ 'value' ])
                $option_attr[ 'selected' ] = 'selected';
            
            $option_attr = attributes( $option_attr );
            $options_html .= "<option$option_attr>$option_label</option>";
        }
    }
    
    //      Handle bootstrap's "help text" add-ons
    $help = '';
    if ( !empty( $options[ 'help-inline' ]) || !empty( $options[ 'help-block' ]))
    {
        if ( !empty( $options[ 'help-inline' ]))
        {
            $help .= '<span class="help-inline">'. $options[ 'help-inline' ] .'</span>';
            unset( $options[ 'help-inline' ]);
        }
        if ( !empty( $options[ 'help-block' ]))
        {
            $help .= '<span class="help-block">'. $options[ 'help-block' ] .'</span>';
            unset( $options[ 'help-block' ]);
        }
    }
    
    //      Attributes shortcut
    $attributes = attributes( $options );
    if ( !empty( $required ))
        $attributes .= " required";
    
    return "<select$attributes>$options_html</select>$help";
}



//--------------------------------------------------------------------------------------------------
//      Textearea


/**
 *  Render text input with labe (control group)
 *  NB : requiert la classe "form-horizontal" sur la balise <form>
 *  @return String : rendered html
 */
function textarea( $label, $options = array())
{
    //      Overridable defaults
    $options += array(
        'text' => '',
        'textarea_attributes' => array(
            //'name' => 'my-text-textarea',
            //'id' => '',
            //'class' => '',
        ),
        'label_attributes' => array(
            'class' => 'control-label',
            //'for' => '',
            //'id' => '',
        ),
    );
    extract( $options );
    
    //      Fill-in optional defaults
    if ( !empty( $label ) && empty( $textarea_attributes[ 'placeholder' ]))
        $textarea_attributes[ 'placeholder' ] = strip_tags( $label );
    else if ( empty( $label ) && !empty( $textarea_attributes[ 'placeholder' ]))
        $label = $textarea_attributes[ 'placeholder' ];
    if ( empty( $textarea_attributes[ 'name' ]) && !empty( $textarea_attributes[ 'placeholder' ]))
        $textarea_attributes[ 'name' ] = str_clean( $textarea_attributes[ 'placeholder' ]);
    
    if ( empty( $textarea_attributes[ 'id' ]))
        if ( !empty( $textarea_attributes[ 'name' ]))
            $textarea_attributes[ 'id' ] = str_clean( $textarea_attributes[ 'name' ]);
        else if ( empty( $textarea_attributes[ 'value' ]))
            $textarea_attributes[ 'id' ] = str_clean( $textarea_attributes[ 'placeholder' ]);
        else
            $textarea_attributes[ 'id' ] = str_clean( $textarea_attributes[ 'value' ]);
    
    if ( empty( $label_attributes[ 'for' ]) && !empty( $textarea_attributes[ 'id' ]))
        $label_attributes[ 'for' ] = $textarea_attributes[ 'id' ];
    
    return '
        <div class="control-group">
            <label'. attributes( $label_attributes ) .'>'. $label .'</label>
            <div class="controls">
                '. textarea_raw( $text, $textarea_attributes ) .'
            </div>
        </div>';
}



/**
 *  Render text input
 *  @return String : rendered html
 */
function textarea_raw( $label = '', $options = array())
{
    //      Overridable defaults
    $options += array(
        'text' => '',
        'rows' => 3,
        'required' => '',
        //'name' => '',
        //'id' => '',
        'class' => 'span5',
    );
    
    //      Anticipate attributes shortcut
    $text = $options[ 'text' ];
    unset( $options[ 'text' ]);
    $required = $options[ 'required' ];
    unset( $options[ 'required' ]);
    
    //      "name" must not be empty
    if ( empty( $options[ 'name' ]) && !empty( $label ))
        $options[ 'name' ] = str_clean( $label );
    if ( empty( $options[ 'placeholder' ]) && !empty( $label ))
        $options[ 'placeholder' ] = strip_tags( $label );
    
    //      Handle bootstrap's "help text" add-ons
    $help = '';
    if ( !empty( $options[ 'help-inline' ]) || !empty( $options[ 'help-block' ]))
    {
        if ( !empty( $options[ 'help-inline' ]))
        {
            $help .= '<span class="help-inline">'. $options[ 'help-inline' ] .'</span>';
            unset( $options[ 'help-inline' ]);
        }
        if ( !empty( $options[ 'help-block' ]))
        {
            $help .= '<span class="help-block">'. $options[ 'help-block' ] .'</span>';
            unset( $options[ 'help-block' ]);
        }
    }
    
    //      Attributes shortcut
    $attributes = attributes( $options );
    if ( !empty( $required ))
        $attributes .= " required";
    
    return "<textarea$attributes>$text</textarea>$help";
}



//--------------------------------------------------------------------------------------------------
//      Text inputs


/**
 *  Wrapper function
 *  Preset for password inputs
 *  @return String : rendered html
 */
function input_password( $label, $options = array())
{
    //      Overridable defaults
    $options += array(
        'input_attributes' => array(
            'type' => 'password',
        ),
    );
    return input_text( $label, $options );
}



/**
 *  Render text input with labe (control group)
 *  NB : requiert la classe "form-horizontal" sur la balise <form>
 *  @return String : rendered html
 */
function input_text( $label, $options = array())
{
    //      Overridable defaults
    $options += array(
        'input_attributes' => array(
            'type' => 'text',
        ),
        'label_attributes' => array(
            'class' => 'control-label',
        ),
        //      Note : if any of those 2 are set, the text input will be appended or prepended
        //      @see input_text_pended()
        //'prepend' => '',
        //'append' => '',
    );
    extract( $options );
    
    //      Fill-in optional defaults
    if ( !empty( $label ) && empty( $input_attributes[ 'placeholder' ]))
        $input_attributes[ 'placeholder' ] = strip_tags( $label );
    else if ( empty( $label ) && !empty( $input_attributes[ 'placeholder' ]))
        $label = $input_attributes[ 'placeholder' ];
    if ( empty( $input_attributes[ 'name' ]))
        if ( !empty( $label ))
            $input_attributes[ 'name' ] = str_clean( $label );
        else if ( !empty( $input_attributes[ 'placeholder' ]))
            $input_attributes[ 'name' ] = str_clean( $input_attributes[ 'placeholder' ]);
    
    if ( empty( $input_attributes[ 'id' ]))
        if ( !empty( $input_attributes[ 'name' ]))
            $input_attributes[ 'id' ] = str_clean( $input_attributes[ 'name' ]);
        else if ( empty( $input_attributes[ 'value' ]))
            $input_attributes[ 'id' ] = str_clean( $input_attributes[ 'placeholder' ]);
        else
            $input_attributes[ 'id' ] = str_clean( $input_attributes[ 'value' ]);
    
    if ( empty( $label_attributes[ 'for' ]) && !empty( $input_attributes[ 'id' ]))
        $label_attributes[ 'for' ] = $input_attributes[ 'id' ];
    
    //      Handle bootstrap's "help text" add-ons
    $help = '';
    if ( !empty( $input_attributes[ 'help-inline' ]) || !empty( $input_attributes[ 'help-block' ]))
    {
        if ( !empty( $input_attributes[ 'help-inline' ]))
        {
            $help .= '<span class="help-inline">'. $input_attributes[ 'help-inline' ] .'</span>';
            unset( $input_attributes[ 'help-inline' ]);
        }
        if ( !empty( $input_attributes[ 'help-block' ]))
        {
            $help .= '<span class="help-block">'. $input_attributes[ 'help-block' ] .'</span>';
            unset( $input_attributes[ 'help-block' ]);
        }
    }
    
    //      Appended or Prepended text input
    if ( empty( $prepend ) && empty( $append ))
        $input = input( $input_attributes );
    else
        $input = input_text_pended( array(
            'append' => $append,
            'prepend' => $prepend,
            'input_attributes' => array( 'class' => '' ) + $input_attributes,
        ));
    
    //      Follow-up "help text" add-ons
    $input .= $help;
    
    return '
        <div class="control-group">
            <label'. attributes( $label_attributes ) .'>'. $label .'</label>
            <div class="controls">
                '. $input .'
            </div>
        </div>';
}



/**
 *  Render text input raw
 *  @return String : rendered html
 */
function input_text_raw( $label, $input_attributes = array())
{
    //      Overridable defaults
    $input_attributes += array(
        'type' => 'text',
        //'placeholder' => 'My text input',
        //'name' => 'my-text-input',
        //'id' => '',
        //'value' => '',
        //'class' => '',
    );
    
    //      Fill-in optional defaults
    if ( !empty( $label ) && empty( $input_attributes[ 'placeholder' ]))
        $input_attributes[ 'placeholder' ] = strip_tags( $label );
    else if ( empty( $label ) && !empty( $input_attributes[ 'placeholder' ]))
        $label = $input_attributes[ 'placeholder' ];
    if ( empty( $input_attributes[ 'name' ]))
        if ( !empty( $label ))
            $input_attributes[ 'name' ] = str_clean( $label );
        else if ( !empty( $input_attributes[ 'placeholder' ]))
            $input_attributes[ 'name' ] = str_clean( $input_attributes[ 'placeholder' ]);
    
    if ( empty( $input_attributes[ 'id' ]))
        if ( !empty( $input_attributes[ 'name' ]))
            $input_attributes[ 'id' ] = str_clean( $input_attributes[ 'name' ]);
        else if ( empty( $input_attributes[ 'value' ]))
            $input_attributes[ 'id' ] = str_clean( $input_attributes[ 'placeholder' ]);
        else
            $input_attributes[ 'id' ] = str_clean( $input_attributes[ 'value' ]);
    
    //      Handle bootstrap's "help text" add-ons
    if ( !empty( $input_attributes[ 'help-inline' ]) || !empty( $input_attributes[ 'help-block' ]))
    {
        $help = '';
        if ( !empty( $input_attributes[ 'help-inline' ]))
        {
            $help .= '<span class="help-inline">'. $input_attributes[ 'help-inline' ] .'</span>';
            unset( $input_attributes[ 'help-inline' ]);
        }
        if ( !empty( $input_attributes[ 'help-block' ]))
        {
            $help .= '<span class="help-block">'. $input_attributes[ 'help-block' ] .'</span>';
            unset( $input_attributes[ 'help-block' ]);
        }
        return input( $input_attributes ) . $help;
    }
    
    return input( $input_attributes );
}



/**
 *  Prepended and appended inputs
 *  
 *  @param Array $options - overrides
 *  @return String : rendered HTML of form element
 *  
 *  @see http://twitter.github.io/bootstrap/base-css.html#forms
 */
function input_text_pended( $options = array())
{
    //      Overridable defaults
    $options += array(
        //'prepend' => '',
        //'append' => '',
        'input_attributes' => array(
            'type' => 'text',
            //'name' => '',
            //'id' => '',
            //'value' => '',
            //'class' => 'span12',
        ),
        'wrapper_additional_attributes' => '',
    );
    extract( $options );
    $html = "";
    
    $wrapper_class_arr = array();
    if ( !empty( $prepend ))
        $wrapper_class_arr[] = "input-prepend";
    if ( !empty( $append ))
        $wrapper_class_arr[] = "input-append";
    $html .= '<div class="'. join( ' ', $wrapper_class_arr ) .'" '. $wrapper_additional_attributes .'>';
    
    $input = '';
    
    if ( !empty( $prepend ))
        $input .= '<span class="add-on">'. $prepend .'</span>';
    
    $input .= input( $input_attributes );
    
    if ( !empty( $append ))
        $input .= '<span class="add-on">'. $append .'</span>';
    
    $html .= $input;
    $html .= '</div>';
    
    return $html;
}



//--------------------------------------------------------------------------------------------------
//      Radio inputs


/**
 *  Helper wrapper function
 *  @return String : rendered html
 */
function input_radio( $label, $options = array())
{
    //      Overridable defaults
    $options += array(
        'type' => 'radio',
    );
    return input_rcb( $label, $options );
}



/**
 *  Radio inputs group
 *  @return String : rendered html
 */
function input_radios( $label, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'inline' => false,
        
        //      The 'name' for all inputs in that group
        //'name' => null,
    );
    extract( $options );
    
    if ( empty( $name ))
        $name = str_clean( $label );
    
    return '
    <div class="control-group">
        <label class="control-label">'. $label .'</label>
        <div class="controls">
            '. input_radios_raw( $name, $options_arr, $options ) .'
        </div>
    </div>
    ';
}



/**
 *  Radio inputs group
 *  @return String : rendered html
 */
function input_radios_raw( $name, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'inline' => false,
    );
    extract( $options );
    
    $html = '';
    if ( !empty( $options_arr ) && is_array( $options_arr ))
    {
        //      Note : if $key is a string, it will be used as the 'value' html attribute
        foreach( $options_arr as $key => $val )
        {
            //      Polymorphism - $val is either :
            //          an array (needs to have at least a 'label' key, the rest are html attributes overrides for this input)
            //          a string -> the label
            if ( !empty( $val ))
            {
                $attributes = array();
                if ( !empty( $name ))
                    $attributes[ 'name' ] = str_clean( $name );
                if ( is_string( $key ))
                    $attributes[ 'value' ] = $key;
                if ( $inline )
                    $attributes[ 'label_class' ] = "inline";
                    
                if ( is_array( $val ) && !empty( $val[ 'label' ]))
                {
                    $label = $val[ 'label' ];
                    unset( $val[ 'label' ]);
                    
                    if ( empty( $attributes[ 'value' ]))
                        $attributes[ 'value' ] = str_clean( $label );
                    
                    $html .= input_radio( $label, $val + $attributes );
                }
                else if ( is_string( $val ))
                {
                    if ( empty( $attributes[ 'value' ]))
                        $attributes[ 'value' ] = str_clean( $val );
                    
                    $html .= input_radio( $val, $attributes );
                }
            }
        }
    }
    
    return $html;
}



//--------------------------------------------------------------------------------------------------
//      Checkbox inputs


/**
 *  Helper wrapper function
 *  @return String : rendered html
 */
function input_checkbox( $label, $options = array())
{
    //      Overridable defaults
    $options += array(
        'type' => 'checkbox',
    );
    return input_rcb( $label, $options );
}



/**
 *  Checkbox inputs group
 *  @return String : rendered html
 */
function input_checkboxes( $label, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'inline' => false,
        
        //      The 'name' for all inputs in that group
        //'name' => null,
    );
    extract( $options );
    
    if ( empty( $name ))
        $name = str_clean( $label );
    
    return '
    <div class="control-group">
        <label class="control-label">'. $label .'</label>
        <div class="controls">
            '. input_checkboxes_raw( $label, $options_arr, $options ) .'
        </div>
    </div>
    ';
}



/**
 *  Checkbox inputs group
 *  @return String : rendered html
 */
function input_checkboxes_raw( $label, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'inline' => false,
    );
    extract( $options );
    
    if ( empty( $name ))
        $name = str_clean( $label );
    
    $html = '';
    if ( !empty( $options_arr ) && is_array( $options_arr ))
    {
        //      Note : if $key is a string, it will be used as the 'value' html attribute
        foreach( $options_arr as $key => $val )
        {
            //      Polymorphism - $val is either :
            //          an array (needs to have at least a 'label' key, the rest are html attributes overrides for this input)
            //          a string -> the label
            if ( !empty( $val ))
            {
                $attributes = array();
                $attributes[ 'name' ] = "{$name}[]";
                
                if ( is_string( $key ))
                    $attributes[ 'value' ] = $key;
                if ( $inline )
                    $attributes[ 'label_class' ] = "inline";
                    
                if ( is_array( $val ) && !empty( $val[ 'label' ]))
                {
                    $label = $val[ 'label' ];
                    unset( $val[ 'label' ]);
                    
                    if ( empty( $attributes[ 'value' ]))
                        $attributes[ 'value' ] = str_clean( $label );
                    
                    $html .= input_checkbox( $label, $val + $attributes );
                }
                else if ( is_string( $val ))
                {
                    if ( empty( $attributes[ 'value' ]))
                        $attributes[ 'value' ] = str_clean( $val );
                    
                    $html .= input_checkbox( $val, $attributes );
                }
            }
        }
    }
    
    return $html;
}



/**
 *  Helper wrapper function
 *  @return String : rendered html
 */
function input_checkboxes_inline( $name, $options_arr, $options = array())
{
    //      Overridable defaults
    $options += array(
        'inline' => true,
    );
    return input_checkboxes( $name, $options_arr, $options );
}



//--------------------------------------------------------------------------------------------------
//      Radio / Checkbox helper


/**
 *  Render checkbox / radio input (generic helper)
 *  
 *  @param Array $options - overrides
 *  @return String : rendered HTML of form element
 *  
 *  @example input_rcb( 'Option', array( 'value' => 'ok', 'checked' => true ))
 *  @example input_rcb( 'Option', array( 'type' => 'radio', 'name' => 'radio_option' ))
 *  @see http://twitter.github.io/bootstrap/base-css.html#forms
 */
function input_rcb( $label, $options = array())
{
    $html = "";
    
    //      Overridable defaults
    $options += array(
        //'id' => null,
        //'class' => null,
        //'type' => 'radio',
        'type' => 'checkbox',
        //'name' => null,
        //'value' => null,
        //'checked' => null,
        //'label_class' => null,
        //'prefix' => null,
        //'suffix' => null,
        //'required' => null,
    );
    extract( $options );
    
    //      Failsafe
    if ( $type != 'checkbox' && $type != 'radio' )
        return '';
    
    //      At least one of those is required
    if ( empty( $label ) && empty( $name ))
        return '';
    
    //      No name ? -> Make one using the label
    if ( empty( $name ))
        $name = str_clean( $label );
    
    //      No value ? -> use name
    if ( empty( $value ))
        $value = $name;
    
    //      Prefix
    if ( !empty( $prefix ))
        $html .= $prefix;
    
    if ( !empty( $label ))
    {
        $lc = $type;
        if ( !empty( $label_class ))
            $lc .= ' '. $label_class;
        if ( !empty( $id ))
            $html .= '<label for="'. $id .'" class="'. $lc .'">';
        else
            $html .= '<label class="'. $lc .'">';
    }
    
    $html .= '<input type="'. $type .'" name="'. $name .'"';
    
    if ( !empty( $id ))
        $html .= ' id="'. $id .'"';
    
    //      HTML 5
    if ( !empty( $checked ))
        $html .= ' checked';
    if ( !empty( $required ))
        $html .= ' required';
    
    $html .= ' value="'. $value .'">';
    
    if ( !empty( $label ))
    {
        $html .= $label;
        $html .= '</label>';
    }
    
    //      Suffix
    if ( !empty( $suffix ))
        $html .= $suffix;

    return $html;
}



//--------------------------------------------------------------------------------------------------
//      Fieldsets


/**
 *  Render checkbox / radio inputs group, wrapped inside a fieldset
 *  
 *  @param Array $options - overrides
 *  @return String : rendered HTML of form element
 *  
 *  @example    select_group( array( 'type' => "checkbox" ))
 */
function fieldset( $legend, $html_content, $options = array())
{
    //      Overridable defaults
    $options += array(
        'fieldset_attributes' => array(
            //'id' => "",
        ),
        'legend_attributes' => array(
            //'id' => "",
        ),
    );
    extract( $options );
    return "<fieldset". attributes( $fieldset_attributes ) ."><legend". attributes( $legend_attributes ) .">$legend</legend>$html_content</fieldset>";
}


