<?php

/**
 *  @file
 *  Pages base template
 */

?><!DOCTYPE html>
<html lang="<?php print $lang; ?>" class="no-js">
<head>
    <?php include 'meta.inc'; ?>
    <?php include 'assets_head.inc'; ?>
    <?php print $head_extra_markup; ?>
</head>
<body<?php if ( !empty( $body_extra_css_classes )) print " class='$body_extra_css_classes'"; if ( !empty( $body_id )) print " id='$body_id'"; ?>>
    
    <!-- navbar -->
    <?php include 'page_navbar.inc'; ?>
    <!-- /navbar -->
    
    <!-- content -->
    <div class="container" id="content">
        <?php print $content_before; ?>
        <?php print $content_markup; ?>
        <?php print $content_after; ?>
    </div>
    <!-- /content -->
    
    <!-- footer -->
    <?php include 'page_footer.inc'; ?>
    <!-- /footer -->
    
    <!-- scripts -->
    <?php include 'assets_bottom.inc'; ?>
    <?php print $footer_extra_js; ?>
    <!-- /scripts -->
    
</body>
</html>