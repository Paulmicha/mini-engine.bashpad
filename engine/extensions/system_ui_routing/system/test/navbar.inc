<?php

/**
 *  @file
 *  Test : Navbar items
 *  @path /system/test/navbar
 */

//      Init
$head_page_title = "Testing navbar";

//      Explanation
$content .= "<h3>The result of these snippets is visible in the navbar <i class='icon-arrow-up'></i></h3>";
$content .= '<p>... we use global scope everywhere, so the <code>$navbar_items</code> variable can be set and/or appended from just about anywhere.</p>';
$content .= '<p>The recommended place for this, though, would be <code>routing/global.inc</code></p>';
$content .= '<hr>';

//      Input text
$code = <<<'EOT'
//      Custom navbar items
//      -> Simple example :
$navbar_items .= menu_item( "Hello menu item", "menu/item", '<i class="icon-remove"></i>' );

//      -> Complete example :
$navbar_items .= '
<li class="dropdown'. ( path_is_in_base_route( 'root-path' ) ? ' active' : '' ) .'">
    <a href="#" class="dropdown-toggle" data-toggle="dropdown"> Parent menu item <b class="caret"></b></a>
    <ul class="dropdown-menu">
        
        '. menu_item( "Hello menu item", "root-path/menu/item", '<i class="icon-remove"></i>' ) .'
        
        <li class="nav-header"> My "nav-header" </li>
        <li class="dropdown-submenu">
            <a href="#"> Dropdown-submenu </a>
            <ul class="dropdown-menu">
                '. menu_item( "Hello submenu item", "root-path/submenu/item", '<i class="icon-remove"></i>' ) .'
            </ul>
        </li>
        
        <li class="divider"></li>
        <li class="nav-header">Simple level 1 items</li>
        '. menu_item( "Level 1 item", "root-path/level-1-item", '<i class="icon-cogs"></i>' ) .'
        
    </ul>
</li>
';
EOT;
$content .= view_source( $code );


//      Custom navbar items
//      -> Simple example :
$navbar_items .= menu_item( "Hello menu item", "menu/item", '<i class="icon-remove"></i>' );

//      -> Complete example :
$navbar_items .= '
<li class="dropdown'. ( path_is_in_base_route( 'root-path' ) ? ' active' : '' ) .'">
    <a href="#" class="dropdown-toggle" data-toggle="dropdown"> Parent menu item <b class="caret"></b></a>
    <ul class="dropdown-menu">
        
        '. menu_item( "Hello menu item", "root-path/menu/item", '<i class="icon-remove"></i>' ) .'
        
        <li class="nav-header"> My "nav-header" </li>
        <li class="dropdown-submenu">
            <a href="#"> Dropdown-submenu </a>
            <ul class="dropdown-menu">
                '. menu_item( "Hello submenu item", "root-path/submenu/item", '<i class="icon-remove"></i>' ) .'
            </ul>
        </li>
        
        <li class="divider"></li>
        <li class="nav-header">Simple level 1 items</li>
        '. menu_item( "Level 1 item", "root-path/level-1-item", '<i class="icon-cogs"></i>' ) .'
        
    </ul>
</li>
';


