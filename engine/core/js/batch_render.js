
/**
 *  @file
 *  Batch API js
 */

//      Global killswitch
//      (for preventing Batch to keep looping)
window.batch_killswitch = false;


/**
 *  Batch ajax calls loop
 */
function execute_batch( o )
{
    //      Expected object properties :
    //          o.step
    //          o.batch_url
    //          o.total
    //          o.id_feedback
    //          o.id_zone
    //          o.delay
    
    //      Feedback
    $( "#" + o.id_feedback ).html( "<div class='alert alert-info'><button type='button' class='close' data-dismiss='alert'>&times;</button><i class='icon-info-sign'></i>&nbsp;Processing " + ( o.step ) + " / " + o.total + "...</div><p style='text-align:center'><i class='icon-spinner icon-spin icon-2x'></i></p>" );
    
    //      Request
    $.ajax({ url: o.batch_url }).done( function( msg )
    {
        //      Display result in specified feedback zone
        $( "#" + o.id_zone ).html( msg );
        o.step++;
        
        //      Chain steps
        if ( o.step < o.total && !window.batch_killswitch )
            setTimeout( function() { execute_batch( o ); }, o.delay );
        
        //      Feedback
        else if ( o.step == o.total )
        {
            $( "#" + o.id_feedback ).html( "<div class='alert alert-info'><button type='button' class='close' data-dismiss='alert'>&times;</button>Process " + ( o.step ) + " / " + o.total + " over. </div>" );
            
            //      Reload when batch is over
            //      update 2013/08/04 20:25:30 - nope (error handling + pause/resume evol possibility)
            //setTimeout( function() { location.reload(); }, o.delay );
        }
        else
            $( "#" + o.id_feedback ).html( "<div class='alert alert-warning'><button type='button' class='close' data-dismiss='alert'>&times;</button>Process " + ( o.step ) + " / " + o.total + " : Pause </div>" );
    });
}


