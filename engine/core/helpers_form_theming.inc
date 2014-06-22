<?php

/**
 *  @file
 *  Form Theming helpers
 */


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
            'value' => 'Submit',
            'class' => 'btn btn-primary btn-large',
        )),
    );
    extract( $options );
    
    //      Hack 2013/09/19 11:02:26 - extra classes (recursive data loss when += array)
    if ( !empty( $class ))
        $attributes[ 'class' ] = $class;
    
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
    if ( @$options[ 'multiple' ] == 'multiple' && !strpos( $options[ 'name' ], '[]' ))
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
            'append' => @$append,
            'prepend' => @$prepend,
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
    
    //      Custom attributes : empty all keys in $options until we only have potential optional attributes
    $keys_to_clean = array( 'id', 'class', 'type', 'name', 'value', 'checked', 'label_class', 'prefix', 'suffix', 'required' );
    foreach( $keys_to_clean as $key )
        if ( isset( $options[ $key ]))
            unset( $options[ $key ]);
    $html .= attributes( $options );
    
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


