<?php

/**
 *  @file
 *  Test : CRUD
 *  @path /system/test/crud
 */


//      Code snippets
$code = <<<'EOT'
//      Init
$head_page_title = "Test CRUD-like functions";

//      Menu tabs
//      Note : "active item" handling is automatic
$content .= nav_tabs( array(
    array( "Source Code", 'system/test/crud' ),
    array( "Create", 'system/test/crud/add' ),
    array( "Read", 'system/test/crud/123' ),
    array( "Update", 'system/test/crud/123/edit' ),
    array( "Delete", 'system/test/crud/123/delete' ),
));



//-------------------------------------------------------------------------------------------
//      Dispatch "Routes"


//      Current path is "/system/test/crud"
//      -> dispatching using 3rd arg
$arg3 = check_plain( arg( 3 ));

//      Section Landing Page
if ( empty( $arg3 ))
{
    $content .= "<h2>Source</h2>";
    $content .= "<p>This complete source code shows :</p>";
    $content .= "<ul><li>how mini-engine could be used to CRUD,</li><li>and how args can be used to make a basic 'route dispatcher'.</li></ul>";
    $content .= "<hr>";
    $content .= view_source( $code );
}


//      Create
else if ( $arg3 == 'add' )
{
    $content .= "<h2>Add entity</h2>";
    
    //      Creation is easy :
    //      Simply render form
    $content .= render_form( _crud_form_content(), '_crud_form_submit', array(
        
        //      Using custom params allows us to re-use validation and submission handlers
        //      for both creation and edition forms
        'submit_handler_params' => array( 'is_new' => 1 ),
        'validation_handler' => '_crud_form_validate',
    ));
}


//      "Selected" an item (by id)
else if ( is_numeric( $arg3 ))
{
    //      Context detection : operation
    $arg4 = check_plain( arg( 4 ));
    
    //      Fake default data for demonstration purpose
    //      Simple real-world example would be :
    //          $entity_data = db_fetch( "SELECT * FROM `entity_db_table` WHERE `id_entity`=$arg3" );
    $entity_data = array(
        'title' => 'Sample fake submission title',
        'description' => 'Sample fake submission description',
        'options_group' => 'YToxOntpOjA7czo4OiJvcHRpb24tMSI7fQ==',
        'radios' => 'option-1',
        'user' => 'paul',
        'password' => 'MY_PASSWORD',
        'select' => 'option-1',
        'multiselect' => 'YToxOntpOjA7czo4OiJvcHRpb24tMSI7fQ==',
        'entity_multigroup' => 'YTo2OntzOjE2OiJtdWx0aV90ZXh0X2lucHV0IjthOjI6e2k6MDtzOjQ0OiJTYW1wbGUgc2Vzc2lvbiBzdWJtaXNzaW9uIG11bHRpIGlucHV0IHRleHQgMSI7aToxO3M6NDQ6IlNhbXBsZSBzZXNzaW9uIHN1Ym1pc3Npb24gbXVsdGkgaW5wdXQgdGV4dCAyIjt9czoxNDoibXVsdGlfdGV4dGFyZWEiO2E6Mjp7aTowO3M6NDI6IlNhbXBsZSBzZXNzaW9uIHN1Ym1pc3Npb24gbXVsdGkgdGV4dGFyZWEgMSI7aToxO3M6NDI6IlNhbXBsZSBzZXNzaW9uIHN1Ym1pc3Npb24gbXVsdGkgdGV4dGFyZWEgMiI7fXM6MTI6Im11bHRpX3NlbGVjdCI7YToyOntpOjA7czo4OiJvcHRpb24tMiI7aToxO3M6ODoib3B0aW9uLTMiO31zOjE3OiJtdWx0aV9tdWx0aXNlbGVjdCI7YToyOntzOjEyOiJtdWx0aWdyb3VwLTAiO2E6MTp7aTowO3M6ODoib3B0aW9uLTIiO31zOjEyOiJtdWx0aWdyb3VwLTEiO2E6MTp7aTowO3M6ODoib3B0aW9uLTMiO319czoxMjoibXVsdGlfcmFkaW9zIjthOjI6e3M6MTI6Im11bHRpZ3JvdXAtMCI7czo4OiJvcHRpb24tMiI7czoxMjoibXVsdGlncm91cC0xIjtzOjg6Im9wdGlvbi0zIjt9czoxNjoibXVsdGlfY2hlY2tib3hlcyI7YToyOntzOjEyOiJtdWx0aWdyb3VwLTAiO2E6MTp7aTowO3M6ODoib3B0aW9uLTIiO31zOjEyOiJtdWx0aWdyb3VwLTEiO2E6MTp7aTowO3M6ODoib3B0aW9uLTMiO319fQ==',
    );
    
    //      Test : if we submitted something in the creation test,
    //      we can easily use the cache to 'properly' test edition as well.
    $cached_test_values = cache_get( 'crud_test_'. str_clean( ip_address()));
    if ( !empty( $cached_test_values ) && is_array( $cached_test_values ))
        $entity_data = $cached_test_values;
    
    //      Read context
    if ( empty( $arg4 ))
    {
        $content .= "<h2>View entity</h2>";
        $content .= "<p>You'd use something like this (and display it whichever way you want) :</p>";
        $content .= view_source( '$entity_data = db_fetch( "SELECT * FROM `entity_db_table` WHERE `id_entity`=$arg3" );' );
        $content .= "<pre>". print_r( $entity_data, 1 ) ."</pre>";
    }
    
    
    //      Update context
    else if ( $arg4 == 'edit' )
    {
        $content .= "<h2>Edit entity</h2>";
        
        //      Mapping loaded values to form inputs ('name' attributes)
        //      Note : form_set_values() should be able to recursievely re-create multi-value fields and groups of fields automatically from arrays
        //      e.g. safe_unserialized() values
        $form_values = array(
            
            'title-of-entity' => $entity_data[ 'title' ],
            'description-of-entity' => $entity_data[ 'description' ],
            'entity-options-group' => safe_unserialize( $entity_data[ 'options_group' ]),
            'entity-radios' => $entity_data[ 'radios' ],
            'user' => $entity_data[ 'user' ],
            'password' => $entity_data[ 'password' ],
            'entity-select' => $entity_data[ 'select' ],
            'entity-multiple-select' => safe_unserialize( $entity_data[ 'multiselect' ]),
            
            //      Note : mutligroups require special treatment
            'multigroups' => array(
                
                //      The key must be the multigroup "namespace" data-attribute
                'entity_multigroup' => safe_unserialize( $entity_data[ 'entity_multigroup' ]),
            ),
        );
        
        //      Render form with its values
        $content .= render_form(
            
            //      This produces the rendered form HTML, with loaded values
            form_set_values( _crud_form_content(), $form_values ),
            
            //      Submit handler
            '_crud_form_submit',
            
            //      Form various options
            array(
                'validation_handler' => '_crud_form_validate',
                
                //      We reuse the 'creation' submit handler -> send 'id_entity' as param
                //      to differenciate usage contexts
                'submit_handler_params' => array( 'id_entity' => $entity_data[ 'id_entity' ]),
                
                'form_attributes' => array(
                    'actions' => input( array(
                            'type' => 'submit',
                            'value' => 'Save',
                            'class' => 'btn btn-primary',
                        )) .
                        " ".
                        l( "Delete", 'system/test/crud/123/delete', array( 'attributes' => array( 'class' => 'btn btn-danger' ))),
                ),
            )
        );
    }
    
    
    //      Delete context
    else if ( $arg4 == 'delete' )
    {
        $content .= "<h2>Delete entity</h2>";
        
        //      Confirmation
        if ( empty( $_GET[ 'yes' ]))
        {
            $content .= "<p>Really delete ?</p>";
            $content .= "<p>";
            $content .= l( "<i class='icon-trash'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-danger' )));
            $content .= "</p>";
        }
        
        //      Deletion
        else if ( $_GET[ 'yes' ] == 'do-it' )
        {
            //      Demo mode
            //$db_delete = db_delete( 'entity', array( 'id' => $id_entity ));
            //if ( !empty( $db_delete ))
                $content .= alert( "Deletion successful", 'success' );
            //else
                //$content .= alert( "Deletion failed. That's all I know.", 'warning' );
        }
    }
}




//-------------------------------------------------------------------------------------------
//      Form CRUD functions


/**
 *  crud edit form
 */
function _crud_form_content()
{
    $form_html = '';
    $form_html .= input_text( 'Title of entity', array( 'input_attributes' => array( 'required' => 1 )));
    $form_html .= textarea( 'Description of entity' );
    $form_html .= input_checkboxes( 'Entity options group', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= input_radios( 'Entity radios', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= input_text( 'User' );
    $form_html .= input_password( 'Password' );
    $form_html .= select( 'Entity select', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= multiple_select( 'Entity multiple Select', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= multigroup( "Entity multigroup", array(
        array(
            'func' => 'input_text',
            'args' => array( "Repeatable text input" ),
        ),
        array(
            'func' => 'textarea',
            'args' => array( "Repeatable textarea" ),
        ),
        array(
            'func' => 'select',
            'args' => array( "Repeatable select", array( 'option 1', 'option 2', 'option 3' )),
        ),
        array(
            'func' => 'select',
            'args' => array( "Repeatable multiple select", array( 'option 1', 'option 2', 'option 3' ), array( 'multiple' => 'multiple' )),
        ),
        array(
            'func' => 'input_radios',
            'args' => array( 'Repeatable Radios Group', array( 'option 1', 'option 2', 'option 3' )),
        ),
        array(
            'func' => 'input_checkboxes',
            'args' => array( 'Repeatable Checkboxes Group', array( 'option 1', 'option 2', 'option 3' )),
        ),
    ));
    return $form_html;
}


/**
 *  crud validation
 */
function _crud_form_validate( $submit_handler_params )
{
    $return = array();
    
    //      Quick sanitization
    $posted = $_POST;
    form_recursive_sanitization( $posted );
    
    //      debug
    //krumo( $posted );
    
    //      Save sanitized values for render_form()
    $return[ 'sanitized_values' ] = $posted;
    
    //      Examples of error checking
    if ( !empty( $posted[ 'title-of-entity' ]))
    {
        //      Test if existing
        //      (If we are in creation context)
        if ( $submit_handler_params[ 'is_new' ])
        {
            //      Sample test
            //$existing_test = db_fetch( 'SELECT * FROM entity WHERE `title` LIKE ?', array( $posted[ 'title' ]));
            //if ( !empty( $existing_test[ 'title' ]))
            //    $return[ 'errors' ] = alert( "It seems this <code>title</code> is already taken : ". $existing_test[ 'title' ], 'warning' );
        }
        
        //      Are we still good to proceed ?
        //      @see _crud_form_submit()
        if ( empty( $return[ 'errors' ]))
        {
            //      Prepare DB record insertion
            $return[ 'values' ] = array(
                'title' => $posted[ 'title-of-entity' ],
                'description' => $posted[ 'description-of-entity' ],
                'options_group' => safe_serialize( $posted[ 'entity-options-group' ]),
                'radios' => $posted[ 'entity-radios' ],
                'user' => $posted[ 'user' ],
                'password' => $posted[ 'password' ],
                'select' => $posted[ 'entity-select' ],
                'multiselect' => safe_serialize( $posted[ 'entity-multiple-select' ]),
                
                //      Note : easier to build 'edit' forms if the keys are the same,
                //      otherwise it's an extra step in the 'mapping' of edit form rendering
                'entity_multigroup' => safe_serialize( array(
                    'multifield-entity-multigroup-repeatable-text-input' => $posted[ 'multifield-entity-multigroup-repeatable-text-input' ],
                    'multifield-entity-multigroup-repeatable-textarea' => $posted[ 'multifield-entity-multigroup-repeatable-textarea' ],
                    'multifield-entity-multigroup-repeatable-select' => $posted[ 'multifield-entity-multigroup-repeatable-select' ],
                    'multifield-entity-multigroup-repeatable-multiple-select' => $posted[ 'multifield-entity-multigroup-repeatable-multiple-select' ],
                    'multifield-entity-multigroup-repeatable-radios-group' => $posted[ 'multifield-entity-multigroup-repeatable-radios-group' ],
                    'multifield-entity-multigroup-repeatable-checkboxes-group' => $posted[ 'multifield-entity-multigroup-repeatable-checkboxes-group' ],
                )),
            );
            
            //      Copy / paste
            //var_export( $return[ 'values' ]);
        }
    }
    
    //      Note : $return[ 'errors' ] can either be an array or a string
    else
        $return[ 'errors' ][] = "Missing 'title'";
    
    return $return;
}



/**
 *  Submit handler
 */
function _crud_form_submit( $params )
{
    //      debug
    //krumo( $params );
    
    //      Example of submit handler
    //      @see _crud_form_validate()
    if ( !empty( $params[ 'values' ]))
    {
        //      This comment is an example of submission handling
        /*
        //      Creation
        if ( $params[ 'is_new' ])
            $id = insert_if_no_existing_record( 'entity', $params[ 'values' ], array(
                'unique_field_name' => 'title',
            ));
        
        //      Edition
        else if ( $params[ 'id_entity' ])
            $id = db_update( 'entity', $params[ 'values' ], array(
                'id' => $params[ 'id_entity' ],
            ));
        
        //      Feedback
        if ( $id )
            return alert( "entity has been saved. <code>id = $id </code>", 'success' );
        */
        
        //      We can easily use the cache to 'properly' test edition as well
        cache_set( 'crud_test_'. str_clean( ip_address()), $params[ 'values' ]);
        
        //      For demonstration purposes
        return alert( "entity has been saved", 'success' ) .'<br><p>Demonstration - form_submit argument "$param" :</p>'. kpr( $params );
    }
    
    return alert( "Error : cannot save the entity. That's all I know.", 'error' );
}
EOT;


//      Init
$head_page_title = "Test CRUD-like functions";

//      Menu tabs
//      Note : "active item" handling is automatic
$content .= nav_tabs( array(
    array( "Source Code", 'system/test/crud' ),
    array( "Create", 'system/test/crud/add' ),
    array( "Read", 'system/test/crud/123' ),
    array( "Update", 'system/test/crud/123/edit' ),
    array( "Delete", 'system/test/crud/123/delete' ),
));



//-------------------------------------------------------------------------------------------
//      Dispatch "Routes"


//      Current path is "/system/test/crud"
//      -> dispatching using 3rd arg
$arg3 = check_plain( arg( 3 ));

//      Section Landing Page
if ( empty( $arg3 ))
{
    $content .= "<h2>Source</h2>";
    $content .= "<p>This complete source code shows :</p>";
    $content .= "<ul><li>how mini-engine could be used to CRUD,</li><li>and how args can be used to make a basic 'route dispatcher'.</li></ul>";
    $content .= "<hr>";
    $content .= view_source( $code );
}


//      Create
else if ( $arg3 == 'add' )
{
    $content .= "<h2>Add entity</h2>";
    
    //      Creation is easy :
    //      Simply render form
    $content .= render_form( _crud_form_content(), '_crud_form_submit', array(
        
        //      Using custom params allows us to re-use validation and submission handlers
        //      for both creation and edition forms
        'submit_handler_params' => array( 'is_new' => 1 ),
        'validation_handler' => '_crud_form_validate',
    ));
}


//      "Selected" an item (by id)
else if ( is_numeric( $arg3 ))
{
    //      Context detection : operation
    $arg4 = check_plain( arg( 4 ));
    
    //      Fake default data for demonstration purpose
    //      Simple real-world example would be :
    //          $entity_data = db_fetch( "SELECT * FROM `entity_db_table` WHERE `id_entity`=$arg3" );
    $entity_data = array(
        'title' => 'Sample fake submission title',
        'description' => 'Sample fake submission description',
        'options_group' => 'YToxOntpOjA7czo4OiJvcHRpb24tMSI7fQ==',
        'radios' => 'option-1',
        'user' => 'paul',
        'password' => 'MY_PASSWORD',
        'select' => 'option-1',
        'multiselect' => 'YToxOntpOjA7czo4OiJvcHRpb24tMSI7fQ==',
        'entity_multigroup' => 'YTo2OntzOjUwOiJtdWx0aWZpZWxkLWVudGl0eS1tdWx0aWdyb3VwLXJlcGVhdGFibGUtdGV4dC1pbnB1dCI7YToyOntzOjEyOiJtdWx0aWdyb3VwLTAiO3M6MjQ6IlNhbXBsZSBmYWtlIHN1Ym1pc3Npb24gMSI7czoxMjoibXVsdGlncm91cC0xIjtzOjI0OiJTYW1wbGUgZmFrZSBzdWJtaXNzaW9uIDIiO31zOjQ4OiJtdWx0aWZpZWxkLWVudGl0eS1tdWx0aWdyb3VwLXJlcGVhdGFibGUtdGV4dGFyZWEiO2E6Mjp7czoxMjoibXVsdGlncm91cC0wIjtzOjI0OiJTYW1wbGUgZmFrZSBzdWJtaXNzaW9uIDEiO3M6MTI6Im11bHRpZ3JvdXAtMSI7czoyNDoiU2FtcGxlIGZha2Ugc3VibWlzc2lvbiAyIjt9czo0NjoibXVsdGlmaWVsZC1lbnRpdHktbXVsdGlncm91cC1yZXBlYXRhYmxlLXNlbGVjdCI7YToyOntzOjEyOiJtdWx0aWdyb3VwLTAiO3M6ODoib3B0aW9uLTIiO3M6MTI6Im11bHRpZ3JvdXAtMSI7czo4OiJvcHRpb24tMyI7fXM6NTU6Im11bHRpZmllbGQtZW50aXR5LW11bHRpZ3JvdXAtcmVwZWF0YWJsZS1tdWx0aXBsZS1zZWxlY3QiO2E6Mjp7czoxMjoibXVsdGlncm91cC0wIjthOjE6e2k6MDtzOjg6Im9wdGlvbi0zIjt9czoxMjoibXVsdGlncm91cC0xIjthOjE6e2k6MDtzOjg6Im9wdGlvbi0zIjt9fXM6NTI6Im11bHRpZmllbGQtZW50aXR5LW11bHRpZ3JvdXAtcmVwZWF0YWJsZS1yYWRpb3MtZ3JvdXAiO2E6Mjp7czoxMjoibXVsdGlncm91cC0wIjtzOjg6Im9wdGlvbi0yIjtzOjEyOiJtdWx0aWdyb3VwLTEiO3M6ODoib3B0aW9uLTMiO31zOjU2OiJtdWx0aWZpZWxkLWVudGl0eS1tdWx0aWdyb3VwLXJlcGVhdGFibGUtY2hlY2tib3hlcy1ncm91cCI7YToyOntzOjEyOiJtdWx0aWdyb3VwLTAiO2E6MTp7aTowO3M6ODoib3B0aW9uLTIiO31zOjEyOiJtdWx0aWdyb3VwLTEiO2E6MTp7aTowO3M6ODoib3B0aW9uLTMiO319fQ',
    );
    
    //      Test : if we submitted something in the creation test,
    //      we can easily use the cache to 'properly' test edition as well.
    $cached_test_values = cache_get( 'crud_test_'. str_clean( ip_address()));
    if ( !empty( $cached_test_values ) && is_array( $cached_test_values ))
        $entity_data = $cached_test_values;
    
    //      debug
    //krumo( $entity_data );
    
    //      Read context
    if ( empty( $arg4 ))
    {
        $content .= "<h2>View entity</h2>";
        $content .= "<p>You'd use something like this (and display it whichever way you want) :</p>";
        $content .= view_source( '$entity_data = db_fetch( "SELECT * FROM `entity_db_table` WHERE `id_entity`=$arg3" );' );
        $content .= "<pre>". print_r( $entity_data, 1 ) ."</pre>";
    }
    
    
    //      Update context
    else if ( $arg4 == 'edit' )
    {
        $content .= "<h2>Edit entity</h2>";
        
        //      Mapping loaded values to form inputs ('name' attributes)
        //      Note : form_set_values() should be able to recursievely re-create multi-value fields and groups of fields automatically from arrays
        //      e.g. safe_unserialized() values
        $form_values = array(
            
            'title-of-entity' => $entity_data[ 'title' ],
            'description-of-entity' => $entity_data[ 'description' ],
            'entity-options-group' => safe_unserialize( $entity_data[ 'options_group' ]),
            'entity-radios' => $entity_data[ 'radios' ],
            'user' => $entity_data[ 'user' ],
            'password' => $entity_data[ 'password' ],
            'entity-select' => $entity_data[ 'select' ],
            'entity-multiple-select' => safe_unserialize( $entity_data[ 'multiselect' ]),
            
            //      Note : mutligroups require special treatment
            'multigroups' => array(
                
                //      The key must be the multigroup "namespace" data-attribute
                'entity_multigroup' => safe_unserialize( $entity_data[ 'entity_multigroup' ]),
            ),
        );
        
        //      debug
        //krumo( $form_values );
        
        //      Render form with its values
        $content .= render_form(
            
            //      This produces the rendered form HTML, with loaded values
            form_set_values( _crud_form_content(), $form_values ),
            
            //      Submit handler
            '_crud_form_submit',
            
            //      Form various options
            array(
                'validation_handler' => '_crud_form_validate',
                
                //      We reuse the 'creation' submit handler -> send 'id_entity' as param
                //      to differenciate usage contexts
                'submit_handler_params' => array( 'id_entity' => @$entity_data[ 'id_entity' ]),
                
                'form_attributes' => array(
                    'actions' => input( array(
                            'type' => 'submit',
                            'value' => 'Save',
                            'class' => 'btn btn-primary',
                        )) .
                        " ".
                        l( "Delete", 'system/test/crud/123/delete', array( 'attributes' => array( 'class' => 'btn btn-danger' ))),
                ),
            )
        );
    }
    
    
    //      Delete context
    else if ( $arg4 == 'delete' )
    {
        $content .= "<h2>Delete entity</h2>";
        
        //      Confirmation
        if ( empty( $_GET[ 'yes' ]))
        {
            $content .= "<p>Really delete ?</p>";
            $content .= "<p>";
            $content .= l( "<i class='icon-trash'></i>&nbsp;Yes, do it", get_current_path() . "?yes=do-it", array( 'attributes' => array( 'class' => 'btn btn-large btn-danger' )));
            $content .= "</p>";
        }
        
        //      Deletion
        else if ( $_GET[ 'yes' ] == 'do-it' )
        {
            //      Demo mode
            //$db_delete = db_delete( 'entity', array( 'id' => $id_entity ));
            //if ( !empty( $db_delete ))
                $content .= alert( "Deletion successful", 'success' );
            //else
                //$content .= alert( "Deletion failed. That's all I know.", 'warning' );
        }
    }
}




//-------------------------------------------------------------------------------------------
//      Form CRUD functions


/**
 *  crud edit form
 */
function _crud_form_content()
{
    $form_html = '';
    $form_html .= input_text( 'Title of entity', array( 'input_attributes' => array( 'required' => 1 )));
    $form_html .= textarea( 'Description of entity' );
    $form_html .= input_checkboxes( 'Entity options group', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= input_radios( 'Entity radios', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= input_text( 'User' );
    $form_html .= input_password( 'Password' );
    $form_html .= select( 'Entity select', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= multiple_select( 'Entity multiple Select', array( 'option 1', 'option 2', 'option 3' ));
    $form_html .= multigroup( "Entity multigroup", array(
        array(
            'func' => 'input_text',
            'args' => array( "Repeatable text input" ),
        ),
        array(
            'func' => 'textarea',
            'args' => array( "Repeatable textarea" ),
        ),
        array(
            'func' => 'select',
            'args' => array( "Repeatable select", array( 'option 1', 'option 2', 'option 3' )),
        ),
        array(
            'func' => 'select',
            'args' => array( "Repeatable multiple select", array( 'option 1', 'option 2', 'option 3' ), array( 'multiple' => 'multiple' )),
        ),
        array(
            'func' => 'input_radios',
            'args' => array( 'Repeatable Radios Group', array( 'option 1', 'option 2', 'option 3' )),
        ),
        array(
            'func' => 'input_checkboxes',
            'args' => array( 'Repeatable Checkboxes Group', array( 'option 1', 'option 2', 'option 3' )),
        ),
    ));
    return $form_html;
}


/**
 *  crud validation
 */
function _crud_form_validate( $submit_handler_params )
{
    $return = array();
    
    //      Quick sanitization
    $posted = $_POST;
    form_recursive_sanitization( $posted );
    
    //      debug
    //krumo( $posted );
    
    //      Save sanitized values for render_form()
    $return[ 'sanitized_values' ] = $posted;
    
    //      Examples of error checking
    if ( !empty( $posted[ 'title-of-entity' ]))
    {
        //      Test if existing
        //      (If we are in creation context)
        if ( $submit_handler_params[ 'is_new' ])
        {
            //      Sample test
            //$existing_test = db_fetch( 'SELECT * FROM entity WHERE `title` LIKE ?', array( $posted[ 'title' ]));
            //if ( !empty( $existing_test[ 'title' ]))
            //    $return[ 'errors' ] = alert( "It seems this <code>title</code> is already taken : ". $existing_test[ 'title' ], 'warning' );
        }
        
        //      Are we still good to proceed ?
        //      @see _crud_form_submit()
        if ( empty( $return[ 'errors' ]))
        {
            //      Prepare DB record insertion
            $return[ 'values' ] = array(
                'title' => $posted[ 'title-of-entity' ],
                'description' => $posted[ 'description-of-entity' ],
                'options_group' => safe_serialize( $posted[ 'entity-options-group' ]),
                'radios' => $posted[ 'entity-radios' ],
                'user' => $posted[ 'user' ],
                'password' => $posted[ 'password' ],
                'select' => $posted[ 'entity-select' ],
                'multiselect' => safe_serialize( $posted[ 'entity-multiple-select' ]),
                
                //      Note : easier to build 'edit' forms if the keys are the same,
                //      otherwise it's an extra step in the 'mapping' of edit form rendering
                'entity_multigroup' => safe_serialize( array(
                    'multifield-entity-multigroup-repeatable-text-input' => $posted[ 'multifield-entity-multigroup-repeatable-text-input' ],
                    'multifield-entity-multigroup-repeatable-textarea' => $posted[ 'multifield-entity-multigroup-repeatable-textarea' ],
                    'multifield-entity-multigroup-repeatable-select' => $posted[ 'multifield-entity-multigroup-repeatable-select' ],
                    'multifield-entity-multigroup-repeatable-multiple-select' => $posted[ 'multifield-entity-multigroup-repeatable-multiple-select' ],
                    'multifield-entity-multigroup-repeatable-radios-group' => $posted[ 'multifield-entity-multigroup-repeatable-radios-group' ],
                    'multifield-entity-multigroup-repeatable-checkboxes-group' => $posted[ 'multifield-entity-multigroup-repeatable-checkboxes-group' ],
                )),
            );
            
            //      Copy / paste
            //var_export( $return[ 'values' ]);
        }
    }
    
    //      Note : $return[ 'errors' ] can either be an array or a string
    else
        $return[ 'errors' ][] = "Missing 'title'";
    
    return $return;
}



/**
 *  Submit handler
 */
function _crud_form_submit( $params )
{
    //      debug
    //krumo( $params );
    
    //      Example of submit handler
    //      @see _crud_form_validate()
    if ( !empty( $params[ 'values' ]))
    {
        //      This comment is an example of submission handling
        /*
        //      Creation
        if ( $params[ 'is_new' ])
            $id = insert_if_no_existing_record( 'entity', $params[ 'values' ], array(
                'unique_field_name' => 'title',
            ));
        
        //      Edition
        else if ( $params[ 'id_entity' ])
            $id = db_update( 'entity', $params[ 'values' ], array(
                'id' => $params[ 'id_entity' ],
            ));
        
        //      Feedback
        if ( $id )
            return alert( "entity has been saved. <code>id = $id </code>", 'success' );
        */
        
        //      We can easily use the cache to 'properly' test edition as well
        cache_set( 'crud_test_'. str_clean( ip_address()), $params[ 'values' ]);
        
        //      For demonstration purposes
        return alert( "entity has been saved", 'success' ) .'<br><p>Demonstration - form_submit argument "$param" :</p>'. kpr( $params );
    }
    
    return alert( "Error : cannot save the entity. That's all I know.", 'error' );
}



