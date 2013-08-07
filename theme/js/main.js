
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
})(jQuery);

