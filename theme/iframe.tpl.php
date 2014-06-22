<?php

/**
 *  @file
 *  Iframes base template
 */

?><!DOCTYPE html>
<html class="no-js">
<head>
    <?php include 'meta.inc'; ?>
    <?php include 'assets_head.inc'; ?>
    <?php print $head_extra_markup; ?>
</head>
<body class="<?php print $body_extra_css_classes; ?>" id="<?php print $body_id; ?>">
    
    <!-- content -->
    <div class="container" id="content">
        <?php print $content_before; ?>
        <?php print $content_markup; ?>
        <?php print $content_after; ?>
    </div>
    <!-- /content -->
    
    <!-- scripts -->
    <?php include 'assets_bottom.inc'; ?>
    <?php print $footer_extra_js; ?>
    <!-- /scripts -->
    
</body>
</html>