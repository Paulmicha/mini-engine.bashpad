<?php

/**
 *  @file
 *  Template Fragment - Meta
 */

//      Prevent html for messy dynamic values
$head_page_title = strip_tags( $head_page_title );
$meta_description = strip_tags( $meta_description );

//      Other meta values
$current_url = get_current_url();

//      Opengraph
if ( empty( $og_description ))
    $og_description = $meta_description;
if ( empty( $og_type ))
    $og_type = 'article';
//if ( empty( $og_image ))
//    $og_image = url( "theme/img/logo.png", array( 'absolute' => true ));

//      Twitter cards
if ( empty( $twitter_description ))
    $twitter_description = $meta_description;
//if ( empty( $twitter_image ))
//    $twitter_image = url( "theme/img/logo.png", array( 'absolute' => true ));


?>
    <meta charset="utf-8">
    
    <title><?php print $head_page_title; ?></title>
    <meta name="description" content="<?php print $meta_description; ?>">
    
    <meta property="og:title" content="<?php print $head_page_title; ?>">
    <meta property="og:description" content="<?php print $og_description; ?>">
    <meta property="og:type" content="<?php print $og_type; ?>">
    <meta property="og:url" content="<?php print $current_url; ?>">
    <?php if ( !empty( $schemaorg_image )) { ?>
    <meta property="og:image" content="<?php print $schemaorg_image; ?>">
    <?php } ?>
    <meta property="og:site_name" content="<?php print $site_name; ?>">
    <!--<meta property="fb:admins" content="your-Facebook-page-user-ID">-->
    
    <meta name="twitter:card" content="summary">
    <meta name="twitter:url" content="<?php print $current_url; ?>">
    <meta name="twitter:title" content="<?php print $head_page_title; ?>">
    <meta name="twitter:description" content="<?php print $twitter_description; ?>">
    <?php if ( !empty( $twitter_image )) { ?>
    <meta name="twitter:image" content="<?php print $twitter_image; ?>">
    <?php } ?>
    