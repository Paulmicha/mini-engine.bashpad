<?php

/**
 *  @file
 *  Pages base template
 */

?><!DOCTYPE html>
<html lang="<?php print $lang; ?>" class="no-js">
<head>
    <?php include 'theme/_meta.inc'; ?>
    <?php include 'theme/_assets_head.inc'; ?>
    <?php print $head_extra_markup; ?>
</head>
<body<?php
if ( !empty( $body_extra_css_classes )) {
    print " class='$body_extra_css_classes'";
}
if ( !empty( $body_id )) {
    print " id='$body_id'";
}
?>>
    
    <?php include 'theme/_page_navbar.inc'; ?>
    
    <!-- content -->
    <div class="container" id="content">
        <?php print $content_before; ?>
        <?php print $content_markup; ?>
        <?php print $content_after; ?>
    </div>
    <!-- /content -->
    
    <?php include 'theme/_page_footer.inc'; ?>
    
    <!-- scripts -->
    <?php include 'theme/_assets_bottom.inc'; ?>
    <?php print $footer_extra_js; ?>
    <!-- /scripts -->
    
</body>
</html>