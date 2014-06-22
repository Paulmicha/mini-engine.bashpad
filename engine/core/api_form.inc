<?php

/**
 *  @file
 *  Form API
 *  
 *  (hacky, quick'n'dirty)
 */


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
        //      @evol 2013/09/20 13:39:57 - possibility to implement custom "hooks" (~events)
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
        
        //      Allow just calling it (without the immadiate return)
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


