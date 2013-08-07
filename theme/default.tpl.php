<?php

/**
 *  @file
 *  Default Page Template (just feedback)
 */

?>
<div class="row-fluid">
    <div class="span12">
        <h1><?php print $head_page_title; ?></h1>
        <?php print $content; ?>
    </div>
</div>
<?php

if ( !empty( $feedback ))
{
    print '<hr />';
    print '<div class="row-fluid"><div class="span12">';
    print '<pre>';
    print $feedback;
    print '</pre>';
    print '</div></div>';
}


