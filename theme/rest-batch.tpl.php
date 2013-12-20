<?php

/**
 *  @file
 *  Batch Rest template
 *  (ajax batch calls)
 */

?>
<div class="row-fluid">
    <div class="span12">
        <?php if ( !empty( $head_page_title )) { ?>
        <p><strong><?php print $head_page_title; ?></strong></p>
        <?php } ?>
        <?php print $content; ?>
    </div>
</div>
<?php

if ( !empty( $feedback ))
{
    print '<hr />';
    print '<div class="row-fluid"><div class="span12">';
    print $feedback;
    print '</div></div>';
}

//      Note : needed to inject javascript during batch
//      Ex: useful for preventing Batch to keep looping (window.batch_killswitch = true;)
print $footer_extra_js;

