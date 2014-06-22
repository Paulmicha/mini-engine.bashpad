<?php

/**
 *  @file
 *  Mini-engine core - optional - system routing
 */

//      Dispatch 'system' routes from core 'routing' directory
if ( arg( 0 ) == 'system' ) {
    $routing_handlers_path = 'engine/core/routing';
}



//-----------------------------------------------------------------------------------------------------------------------
//      Navbar


$navbar_items .= '
    <li class="dropdown'. ( path_is_in_base_route( 'system' ) ? ' active' : '' ) .'">
        <a href="#" class="dropdown-toggle" data-toggle="dropdown"> System <b class="caret"></b></a>
        <ul class="dropdown-menu">
            
            '. ( $enable_db ? '
            <li class="dropdown-submenu">
                <a href="#"> Setup </a>
                <ul class="dropdown-menu">
                    '. menu_item( "Install Database Tables <sup class='text-error'><i class='icon-asterisk'></i></sup>", "system/setup/install", '<i class="icon-download-alt"></i>' ) .'
                    '. menu_item( "Drop All tables", "system/setup/uninstall", '<i class="icon-trash"></i>' ) .'
                    <li class="divider"></li>
                    <li class="nav-header"><sup><i class="icon-asterisk"></i></sup> : Mandatory to execute at least once.</li>
                </ul>
            </li>
            <li class="dropdown-submenu">
                <a href="#"> Configuration </a>
                <ul class="dropdown-menu">
                    '. menu_item( "Load config <sup class='text-error'><i class='icon-asterisk'></i></sup>", "system/config/load", '<i class="icon-upload"></i>' ) .'
                    '. menu_item( "Save config", "system/config/save", '<i class="icon-download-alt"></i>' ) .'
                </ul>
            </li>
            <li class="dropdown-submenu">
                <a href="#"> System Tools </a>
                <ul class="dropdown-menu">
                    '. menu_item( "Inspect DB Tables", "system/inspect-table", '<i class="icon-zoom-in"></i>' ) .'
                    '. ( $enable_cache ? menu_item( "Inspect Cache", "system/cache-entries", '<i class="icon-ambulance"></i>' ) : '' ) .'
                </ul>
            </li>
            <li class="divider"></li>
            ' : '' ) .'
            
            <li class="dropdown-submenu">
                <a href="#"> Tests (self-doc, snippets) </a>
                <ul class="dropdown-menu">
                    '. menu_item( "URL / Routing", "system/test/routing", '<i class="icon-random"></i>' ) .'
                    '. menu_item( "Reserved PHP variables", "system/test/globals", '<i class="icon-usd"></i>' ) .'
                    '. menu_item( "Utilities", "system/test/utilities", '<i class="icon-wrench"></i>' ) .'
                    '. ( $enable_db ? menu_item( "DB API", "system/test/db", '<i class="icon-cogs"></i>' ) : '' ) .'
                    '. menu_item( "UI helpers", "system/test/theming", '<i class="icon-pencil"></i>' ) .'
                    '. menu_item( "Navbar", "system/test/navbar", '<i class="icon-pencil"></i>' ) .'
                    '. menu_item( "Icons", "system/test/icons", '<i class="icon-flag"></i>' ) .'
                    '. ( $enable_file ? menu_item( "File", "system/test/file", '<i class="icon-file"></i>' ) : '' ) .'
                    '. ( $enable_cache ? menu_item( "Cache", "system/test/cache", '<i class="icon-cogs"></i>' ) : '' ) .'
                    '. ( $enable_cache ? menu_item( "Cache clear", "system/test/cache-clear", '<i class="icon-cogs"></i>' ) : '' ) .'
                    '. ( $enable_dblog ? menu_item( "DBLog", "system/test/dblog", '<i class="icon-info-sign"></i>' ) : '' ) .'
                    '. ( $enable_datatable ? menu_item( "Data table", "system/test/datatable", '<i class="icon-th"></i>' ) : '' ) .'
                    '. ( $enable_form_theming ? menu_item( "Form theming", "system/test/form-theming", '<i class="icon-cogs"></i>' ) : '' ) .'
                    '. ( $enable_form_multigroup ? menu_item( "Form multigroup", "system/test/form-multigroup", '<i class="icon-cogs"></i>' ) : '' ) .'
                    '. ( $enable_form_file_selector ? menu_item( "Form file selector", "system/test/form-file-selector", '<i class="icon-file"></i>' ) : '' ) .'
                    '. ( $enable_form ? menu_item( "Form handling", "system/test/form-handling", '<i class="icon-cogs"></i>' ) : '' ) .'
                    '. ( $enable_ssh ? menu_item( "SSH / SFTP", "system/test/phpseclib", '<i class="icon-lock"></i>' ) : '' ) .'
                </ul>
            </li>
            
            '. ( $enable_tag ? '
            <li class="divider"></li>
            '. menu_item( "Browse Tags", "system/tags", '<i class="icon-tags"></i>' ) .'
            ' : '' ) .'
            
            '. ( $enable_dblog ? '
            <li class="divider"></li>
            '. menu_item( "DB Log entries", "system/dblog", '<i class="icon-info-sign"></i>' ) .'
            ' : '' ) .'
            
        </ul>
    </li>
'; 


