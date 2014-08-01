<?php

/**
 *  @file
 *  Navbar template
 *  @require Bootstrap-2.3.2
 */

?>
<!-- navbar -->
<div class="container">
    <div class="navbar navbar-inverse">
        <div class="navbar-inner">
            <div class="container">
                <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </a>
                
                <!--
                <a class="brand" href="<?php print $base_path; ?>"><?php print $short_page_title; ?></a>
                -->
                
                <div class="nav-collapse collapse">
                    <ul class="nav">
                        <li><?php print l( '<i class="icon-home"></i>', '' ); ?></li>
                        <?php
                        
                        //      Menu Item Syntax :
                        //      <li><a href="#test"> Test </a></li>
                        
                        //      Adding items in navbar through $navbar_items
                        if ( !empty( $navbar_items )) {
                            print $navbar_items;
                        }
                        
                        ?>
                    </ul>
                </div>
                
                <ul class="nav pull-right">
                    <li><small>You are <code><?php print ip_address(); ?></code></small></li>
                    <?php
                    
                    //      Utility reload link when data has been posted
                    //      (and we don't want to re-post, which happens when we hit brower refresh)
                    if ( !empty( $_POST )) {
                    ?>
                    <li><?php print reload_link( '<small>Reload without re-posting</small>' ); ?></li>
                    <?php
                    }
                    ?>
                </ul>
                
            </div>
        </div>
    </div>
</div>
<!-- /navbar -->