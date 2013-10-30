<?php

/**
 *  @file
 *  Page Template - homepage (do not remove)
 */

print grid( array(
    array(
        
        "<h1>About</h1>
<p>This project is a minimalist Bash commands <strong><em>store</em></strong> and <strong><em>launch pad</em></strong>.</p>
<p>It's currently built on top of a custom Php app from scratch,
which was originally designed for automating repetitive tasks (such as data preparation for migrations).<br>
It should be rebuild in modern PHP - for ex. using Symfony 2 components, but its current state can be seen as a <em>museum</em> of Php snippets put together (hence <em>Here be dragons</em>).</p>
<p><strong><i class='icon-warning-sign'></i> WARNING</strong> : this is probably full of massive security breaches, as it was never meant to be publicly accessible over the web.<br>
The reason is mainly because the underlying old code base wasn't built for it, and because it can connect to other servers through SSH in order to execute Bash commands... In short, <strong>NEVER run this 'outdoors'</strong>.</p>",
        
        "<h2>Quick start</h2>
<ol>
    <li>Edit the settings in <code>engine/settings.inc</code>, and adjust <code>.htaccess</code> file for the rewrite rules if needed;</li>
    <li>Install (once) DB Tables in ". l( 'system/setup/install', 'system/setup/install' ) .";</li>
    <li>Import (once) defaults in ". l( 'system/setup/load-configuration', 'system/setup/load-configuration' ) .";</li>
    <li>Setup your server(s) in ". l( 'system/server/add', 'system/server/add' ) .";</li>
    <li>Define a few environments &amp; folder paths on each server created</li>
    <li>Manage available Bash command(s) in ". l( 'system/command', 'system/command' ) .";</li>
    <li>Setup Batch(es) in ". l( 'batch/add', 'batch/add' ) .";</li>
    <li>Pick and launch your Batch(es) in ". l( 'batch', 'batch' ) .";</li>
    <li>Review execution logs (std out) in ". l( 'batch/archives', 'batch/archives' ) .".</li>
</ol>
<p>Also, you'll want to quickly hack something, so have a look at the snippets in the <code>System</code> menu to reuse available 'helpers'.
Examples :</p>
<ul class='inline'>
    ". menu_item( "URL / Routing", "system/test/routing", '<i class="icon-random"></i>' ) ."
    ". menu_item( "Reserved PHP variables", "system/test/globals", '<i class="icon-usd"></i>' ) ."
    ". menu_item( "Utilities", "system/test/utilities", '<i class="icon-wrench"></i>' ) ."
    ". menu_item( "DB API", "system/test/db", '<i class="icon-cogs"></i>' ) ."
    ". menu_item( "UI helpers", "system/test/theming", '<i class="icon-pencil"></i>' ) ."
    ". menu_item( "Navbar", "system/test/navbar", '<i class="icon-pencil"></i>' ) ."
    ". menu_item( "Icons", "system/test/icons", '<i class="icon-flag"></i>' ) ."
    ". menu_item( "File", "system/test/file", '<i class="icon-file"></i>' ) ."
    ". menu_item( "Cache", "system/test/cache", '<i class="icon-cogs"></i>' ) ."
    ". menu_item( "Cache clear", "system/test/cache-clear", '<i class="icon-cogs"></i>' ) ."
    ". menu_item( "DBLog", "system/test/dblog", '<i class="icon-info-sign"></i>' ) ."
    ". menu_item( "Simple 'views'", "system/test/simple-views", '<i class="icon-th"></i>' ) ."
    ". menu_item( "Form", "system/test/forms", '<i class="icon-cogs"></i>' ) ."
    ". menu_item( "CRUD", "system/test/crud", '<i class="icon-rocket"></i>' ) ."
    ". menu_item( "SSH / SFTP", "system/test/phpseclib", '<i class="icon-lock"></i>' ) ."
</ul>
<!--
<ul>
    <li><i class=\"icon-hand-up icon-large\"></i> have a look at the snippets in the <code>System</code> menu to reuse available 'helpers'.</li>
    <li>Pick a path to start with &ndash; e.g. for ". l( "/test", "test" ) ." you need to create <code>context/test.inc</code>.<br>In this file, set these 2 variables, like : <code>\$head_page_title = 'My page title';</code> and <code>\$content .= \"Hello world\";</code>.<br>
        Once this is done, ". l( "have a look at the result", 'test' ) .".</li>
    <li>You may provide \"Navbar\" items in <code>context/global.inc</code></li>
</ul>
-->
"
    ),
));

?>
<hr>
<h2>Simplified Schema</h2>
<p>Illustrating briefly the database structure, and generally, the underlying organization of this "mini-engine".</p>
<div style="text-align:center"><img src="<?php print $base_path; ?>theme/img/mini-engine-schema-v07.png" alt="schéma des entités telles qu'attendues par Drupal" /></div>
