<?php

/**
 *  @file
 *  Iframes base template
 */

?><!DOCTYPE html>
<html class="no-js">
<head>
    <?php include 'theme/_meta.inc'; ?>
    <?php include 'theme/_assets_head.inc'; ?>
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
    <?php include 'theme/_assets_bottom.inc'; ?>
    <?php print $footer_extra_js; ?>
    <!-- /scripts -->
    
</body>
</html>