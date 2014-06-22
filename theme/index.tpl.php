<?php

/**
 *  @file
 *  Page Template - homepage (do not remove)
 */

print grid( array(
    array(
        
        "<h1>About</h1>
<p>This project is a minimalist Bash commands <strong><em>store</em></strong> and <strong><em>launch pad</em></strong>.</p>
<p>It's currently built on top of a custom Php app from scratch, which was originally designed for automating repetitive tasks (such as data preparation for migrations). It should be rebuilt in modern PHP - for ex. using Symfony 2 components, and its current state can be seen as my personal <em>museum</em> of Php snippets put together (hence <em>Here be dragons</em>).</p>
<p><strong><i class='icon-warning-sign'></i> WARNING</strong> : this application will contain extremely sensitive & unobfuscated data, and it was never meant to be publicly accessible over the web (because it can connect to other servers through SSH in order to execute Bash commands). It is probably unsecure, because the underlying old code base wasn't built for it : we only use it in LAN for speeding up repetitive tasks on our development environments, and I would strongly advise <strong>not</strong> to host it anywhere accessible online.</p>",
        
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
<p>To quickly hack something without rewriting existing functionalities, have a look at the snippets in the <code>System</code> menu to reuse available 'helpers'.
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
"
    ),
));

?>
<hr>
<h2>Simplified Schema</h2>
<p>Illustrating briefly the database structure, and generally, the underlying organization of this "mini-engine".</p>
<div style="text-align:center"><img src="<?php print $base_path; ?>theme/img/mini-engine-schema-v07.png" alt="Mini-engine : Simplified Entities Schema" /></div>
