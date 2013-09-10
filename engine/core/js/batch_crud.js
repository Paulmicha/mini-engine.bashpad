
/**
 *  @file
 *  Batch API js - CRUD
 */

(function($)
{
    /**
     *  Listen for multigroups additions
     */
    function batch_step_multigroup_repeat_listener()
    {
        //      Init autocomplete
        $( 'input.autocompletion-command' ).typeahead({ source:window.autocomplete_cmd_arr });
        
        //      Chosen selects must be fixed (initialised when hidden = null width) :
        //      whenever any initially hidden select field become visible for the first time, re-apply chosen.
        $( 'a[data-toggle="tab"]' ).each( function()
        {
            //      Init event listener (only once)
            if ( !$(this).data( 'fix_chosen_listener_initialized' ))
            {
                //      Listen for clicks on tab control, fix any 'candidate' once if needed & remove listener when done.
                //      @see http://getbootstrap.com/2.3.2/javascript.html#tabs
                $(this).on( 'shown', function( e )
                {
                    //      Force 50% width on tab contents' child chosen containers
                    $( e.target ).parent().parent().parent().find( '.tab-pane.active .chzn-container' ).css( 'width', '50%' );
                    
                    //      Remove this listener when done
                    $(this).off( 'shown' );
                });
                
                //      Set "flag" for initializing this only once
                $(this).data( 'fix_chosen_listener_initialized', 1 );
            }
        });
        
        //      Replace 'href' attributes for inner left vertical tabs controls,
        //      And 'id' attributes for corresponding tabs container elements
        //      @see engine/core/js/repeatable_form_fields.js
        if ( window.last_appended_element && window.last_appended_element.length > 0 )
        {
            //      Antipattern : using global scope
            $( 'a[data-toggle="tab"]', window.last_appended_element ).each( function()
            {
                $(this).attr( 'href', function( index, href )
                {
                    return href.replace( /multigroup-(\d+)/, function( fullMatch, n ) {
                        return "multigroup-" + window.unique_multigroup_numbering;
                    });
                });
            });
            
            //      Antipattern : using global scope
            $( 'div.tab-pane', window.last_appended_element ).each( function()
            {
                $(this).attr( 'id', function( index, id )
                {
                    return id.replace( /multigroup-(\d+)/, function( fullMatch, n ) {
                        return "multigroup-" + window.unique_multigroup_numbering;
                    });
                });
            });
        }
    }

    //      Global scope custom event binding :
    //      update autocomplete & attributes for tabs & fields in "batch steps" multigroup
    //      @see engine/core/js/repeatable_form_fields.js
    $( "body" ).bind( "form_field_repeated", batch_step_multigroup_repeat_listener );

    //      Init
    batch_step_multigroup_repeat_listener();

})(jQuery);


