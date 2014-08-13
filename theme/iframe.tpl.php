<?php

/**
 *  @file
 *  Iframes base template
 */

?><!DOCTYPE html>
<html<?php print attributes( $html_attr ); ?>>
<head>
    <?php include 'theme/_meta.inc'; ?>
    <?php include 'theme/_assets_head.inc'; ?>
    <?php print $spaces[ 'head' ]; ?>
</head>
<body<?php print attributes( $body_attr ); ?>>
    
    <?php if ( !empty( $content_rendered )) { ?>
    <!-- content -->
    <div class="container" id="content">
        <?php print $content_rendered; ?>
    </div>
    <!-- /content -->
    <?php }//  end if ( !empty( $content_rendered )) ?>
    
    <?php include 'theme/_assets_bottom.inc'; ?>
    <?php print $spaces[ 'page_bottom' ]; ?>
    
</body>
</html>