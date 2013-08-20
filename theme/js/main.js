
/**
 *  @file
 *  Mini engine - main JS
 */

(function($)
{
    //      Chosen
    $( "select" ).chosen({ search_contains:1, disable_search_threshold:7 });
    
    //      Datatables
    $(document).ready(function()
    {
        $('.data-table-hook').dataTable({
            "sDom": "<'row-fluid'<'span6'l><'span6'f>r>t<'row-fluid'<'span6'i><'span6'p>>",
            "sPaginationType": "bootstrap",
            "oLanguage": {
                "sLengthMenu": "_MENU_ records per page"
            },
            
            //      Note : always sort by default by first column in descending order
            "aaSorting": [[ 0, "desc" ]]
        });
    });
    
    //      Depends-on (visibility switch)
    setTimeout( function()
    {
        $( '[data-depends-on]' ).each( function( i )
        {
            var id = $(this).attr( 'data-depends-on' );
            
            //      Hidden by default
            $(this).hide();
            
            //      Save this targetted element as data on its "controller"
            $( '#' + id ).data( 'controlled-target', $(this));
            
            //      @evol 2013/08/20 14:15:28 - this assumes the eleement is a checkbox : make it more generic
            //      (had a snippet somewhere, can't find it)
            $( '#' + id ).change( function()
            {
                if ( this.checked )
                    $(this).data( 'controlled-target' ).show();
                else
                    $(this).data( 'controlled-target' ).hide();
            });
        });
    }, 87 );
    
})(jQuery);

