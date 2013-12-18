<?php

/**
 *  @file
 *  Global functions & vars (always loaded)
 *  Meant for custom appli-specific stuff
 */

//      Global helpers, that are specific to this implementation (table-specific)
//      I put them here because I thought they didn't really belong in "engine" dir
include '_specific_helpers.inc';


//      Custom navbar items
//      -> Simple example :
//$navbar_items .= menu_item( "Hello menu item", "menu/item", '<i class="icon-remove"></i>' );

//      -> Complete example :
/*
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
*/