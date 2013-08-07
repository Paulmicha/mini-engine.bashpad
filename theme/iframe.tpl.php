<?php

/**
 *  @file
 *  Iframes base template
 */

?><!DOCTYPE html>
<html class="no-js">
<head>
    <title><?php print $head_page_title; ?></title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta name="viewport" content="width=device-width">
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