
/**
 *  @file
 *  Repearable form fields
 *  
 *  @see http://jsfiddle.net/Z4kYf/1/
 *  @see http://stackoverflow.com/questions/13536148/jquery-repeatable-form-fields-adding-doesnt-work
 */

(function($)
{
    /**
     *  Add btn
     */
    $('.repeatable-field-add').click( function()
    {
        var multigroup_wrapper = $(this).closest( 'div.repeatable-wrap' );
        var namespace = multigroup_wrapper.attr( 'data-namespace' );
        
        //      Storing raw html inside js variable, because of chosen
        //var theField = $(this).closest( 'div.repeatable-wrap' )
        //    .find( '.repeatable-fields-list div.field-group-inner-wrap:last' )
        //    .clone( true );
        var theField = $( repeatable_fields_html[ namespace ]);
        
        var theLocation = multigroup_wrapper.find( '.repeatable-fields-list div.field-group-inner-wrap:last' );
        
        //      Empty text values if there were any before cloning
        $( 'input[type="text"]', theField ).val( '' );
        $( 'textarea', theField ).text( '' );
        
        //      Determine next unique multigroup number
        stop = false;
        window.unique_multigroup_numbering = 0;
        while( !stop && multigroup_wrapper.find( "[name*='multigroup-" + window.unique_multigroup_numbering + "']" ).length > 0 )
        {
            window.unique_multigroup_numbering++;
            if ( window.unique_multigroup_numbering > 999 )
                stop = true;
        }
        
        //      debug
        //console.log( window.unique_multigroup_numbering );
        
        //      Adjust attributes
        $( 'input,textarea,select,label', theField ).each( function()
        {
            //      Name attributes
            if ( $(this).attr( 'name' ))
            {
                $(this).attr( 'name', function( index, name )
                {
                    return name.replace( /multigroup-(\d+)/, function( fullMatch, n ) {
                        return "multigroup-" + window.unique_multigroup_numbering;
                    });
                });
            }
            
            //      Unique IDs
            if ( $(this).attr( 'id' ))
            {
                $(this).attr( 'id', function( index, id )
                {
                    return id.replace( /multigroup-(\d+)/, function( fullMatch, n ) {
                        return "multigroup-" + window.unique_multigroup_numbering;
                    });
                });
            }
            
            //      Unique "for" attributes
            if ( $(this).attr( 'for' ))
            {
                $(this).attr( 'for', function( index, _for )
                {
                    return _for.replace( /multigroup-(\d+)/, function( fullMatch, n ) {
                        return "multigroup-" + window.unique_multigroup_numbering;
                    });
                });
            }
        });
        
        //      update html "base" string, to increment attributes (id & name) counts
        //console.log( theField[ 0 ].outerHTML );
        repeatable_fields_html[ namespace ] = theField[ 0 ].outerHTML;
        
        //      Append HTML
        var appended_element = theField.insertAfter( theLocation, $(this).closest( 'div.repeatable-wrap' ));
        
        //      Since we use raw HTML inside javascript variable,
        //      all javascript events must be initialized on new "copies"
        //      -> Chosen
        $( "select" ).chosen({ search_contains:1, disable_search_threshold:7 });
        //      -> "Remove" buttons
        $( '.repeatable-field-remove' ).unbind( 'click', remove_handler );
        $( '.repeatable-field-remove' ).bind( 'click', remove_handler );
        
        //      Toggle remove button visibility
        var fieldsCount = $( '.repeatable-field-remove' ).length;
        if ( fieldsCount > 1 ) {
            $('.repeatable-field-remove').css( 'display', 'inline' );
        }
        
        //      Custom event to react to repeatable fields "addition"
        //      @see http://ejohn.org/apps/workshop/adv-talk/#14
        window.last_appended_element = appended_element;
        $( "body" ).trigger( "form_field_repeated" );
        
        return false;
    });
    
})(jQuery);



/**
 *  Remove btn event handler
 */
function remove_handler()
{
    //$(this).parent().remove();
    $(this).parent().parent().remove();
    
    var fieldsCount = $( '.repeatable-field-remove' ).length;
    if ( fieldsCount == 1 ) {
        $( '.repeatable-field-remove' ).css( 'display', 'none' );
    }
    
    return false;
}


