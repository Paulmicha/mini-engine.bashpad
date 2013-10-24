<?php

/**
 *  @file
 *  Template Fragment - Pages Navbar
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
                        //      <li><a href="#contact"> Contact </a></li>
                        ?>
                        
                        <li class="dropdown<?php if ( path_is_in_base_route( 'system' )) { print ' active'; } ?>">
                            <a href="#" class="dropdown-toggle" data-toggle="dropdown"> System <b class="caret"></b></a>
                            <ul class="dropdown-menu">
                                
                                <li class="nav-header">Core</li>
                                <li class="dropdown-submenu">
                                    <a href="#"> Setup </a>
                                    <ul class="dropdown-menu">
                                        <?php print menu_item( "Install SQL tables <sup class='text-error'><i class='icon-asterisk'></i></sup>", "system/setup/install", '<i class="icon-download-alt"></i>' ); ?>
                                        <?php print menu_item( "Load defaults <sup class='text-error'><i class='icon-asterisk'></i></sup>", "system/setup/load-configuration", '<i class="icon-upload"></i>' ); ?>
                                        <li class="divider"></li>
                                        <?php print menu_item( "Drop All tables", "system/setup/uninstall", '<i class="icon-trash"></i>' ); ?>
                                        <li class="divider"></li>
                                        <li class="nav-header"><sup><i class='icon-asterisk'></i></sup> : Mandatory to execute at least once.</li>
                                    </ul>
                                </li>
                                <li class="dropdown-submenu">
                                    <a href="#"> Tools </a>
                                    <ul class="dropdown-menu">
                                        <?php print menu_item( "Inspect DB Tables", "system/inspect-table", '<i class="icon-zoom-in"></i>' ); ?>
                                        <?php print menu_item( "Inspect Cache", "system/cache-entries", '<i class="icon-ambulance"></i>' ); ?>
                                    </ul>
                                </li>
                                
                                
                                <li class="divider"></li>
                                <li class="dropdown-submenu">
                                    <a href="#"> Tests + Snippets </a>
                                    <ul class="dropdown-menu">
                                        <?php print menu_item( "URL / Routing", "system/test/routing", '<i class="icon-random"></i>' ); ?>
                                        <?php print menu_item( "Reserved PHP variables", "system/test/globals", '<i class="icon-usd"></i>' ); ?>
                                        <?php print menu_item( "Utilities", "system/test/utilities", '<i class="icon-wrench"></i>' ); ?>
                                        <?php print menu_item( "DB API", "system/test/db", '<i class="icon-cogs"></i>' ); ?>
                                        <?php print menu_item( "UI helpers", "system/test/theming", '<i class="icon-pencil"></i>' ); ?>
                                        <?php print menu_item( "Navbar", "system/test/navbar", '<i class="icon-pencil"></i>' ); ?>
                                        <?php print menu_item( "Icons", "system/test/icons", '<i class="icon-flag"></i>' ); ?>
                                        <?php print menu_item( "File", "system/test/file", '<i class="icon-file"></i>' ); ?>
                                        <?php print menu_item( "Cache", "system/test/cache", '<i class="icon-cogs"></i>' ); ?>
                                        <?php print menu_item( "Cache clear", "system/test/cache-clear", '<i class="icon-cogs"></i>' ); ?>
                                        <?php print menu_item( "DBLog", "system/test/dblog", '<i class="icon-info-sign"></i>' ); ?>
                                        <?php print menu_item( "Simple 'views'", "system/test/simple-views", '<i class="icon-th"></i>' ); ?>
                                        <?php print menu_item( "Form", "system/test/forms", '<i class="icon-cogs"></i>' ); ?>
                                        <?php print menu_item( "CRUD", "system/test/crud", '<i class="icon-rocket"></i>' ); ?>
                                        <?php print menu_item( "SSH / SFTP", "system/test/phpseclib", '<i class="icon-lock"></i>' ); ?>
                                    </ul>
                                </li>
                                
                                
                                <li class="divider"></li>
                                <?php print menu_item( "Browse Tags", "system/tags", '<i class="icon-tags"></i>' ); ?>
                                
                                
                                <li class="divider"></li>
                                <li class="nav-header">Servers</li>
                                <?php
                                $servers_setup = db_fetchall( "SELECT * FROM `server` ORDER BY `id_server` DESC" );
                                if ( !empty( $servers_setup ))
                                {
                                ?>
                                <li class="dropdown-submenu">
                                    <a href="#"> Manage </a>
                                    <ul class="dropdown-menu">
                                        <?php
                                        foreach( $servers_setup as $server )
                                            print menu_item( $server[ 'title' ], "system/server/". $server[ 'id_server' ], '<i class="icon-wrench"></i>' );
                                        ?>
                                    </ul>
                                </li>
                                <?php
                                }
                                ?>
                                <?php print menu_item( "Add new server", "system/server/add", '<i class="icon-plus"></i>' ); ?>
                                
                                
                                <li class="divider"></li>
                                <li class="nav-header">Commands</li>
                                <?php print menu_item( "Manage commands", "system/command", '<i class="icon-cogs"></i>', array( 'active_callback' => 'path_is_active' )); ?>
                                <?php print menu_item( "Add new command", "system/command/add", '<i class="icon-plus"></i>' ); ?>
                                
                                
                                <li class="divider"></li>
                                <li class="nav-header">Log</li>
                                <?php print menu_item( "DB Log entries", "system/dblog", '<i class="icon-info-sign"></i>' ); ?>
                                
                            </ul>
                        </li>
                        
                        
                        <li class="dropdown<?php if ( path_is_in_base_route( 'batch' )) { print ' active'; } ?>">
                            <a href="#" class="dropdown-toggle" data-toggle="dropdown"> Batch <b class="caret"></b></a>
                            <ul class="dropdown-menu">
                                <?php print menu_item( "Launch operations", "batch", '<i class="icon-rocket"></i>', array( 'active_callback' => 'path_is_active' )); ?>
                                <?php print menu_item( "Create new Batch", "batch/add", '<i class="icon-plus"></i>' ); ?>
                                <?php print menu_item( "Presets", "batch/presets", '<i class="icon-road"></i>' ); ?>
                                <?php print menu_item( "Archives", "batch/archives", '<i class="icon-hdd"></i>' ); ?>
                                <?php print menu_item( "Status", "batch/status", '<i class="icon-dashboard"></i>' ); ?>
                            </ul>
                        </li>
                        
                        
                        <?php
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