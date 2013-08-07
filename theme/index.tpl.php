<?php

/**
 *  @file
 *  Page Template - homepage (do not remove)
 */

print grid( array(
    array(
        
        "<h1>$head_page_title</h1>
<p>This is a legacy minimalist RAD Php application,
which was designed exclusively to execute small batch processes.<br>
It still has its uses : namely to store & execute Bash commands (with tokens)
on a configurable list of servers and environments.<br><br>
I've used it over the years for repetitive tasks around various PHP projects.
It was never meant to be Object-Oriented, and it can be seen as a good source of antipatterns,
but it could be an intersting exercise to rebuild it in \"modern\" PHP, using things like Symfony 2 or Laravel.<br>
Basically, this is a \"museum\" of various Php snippets from a few years back put together.</p>
<p><strong>WARNING</strong> : this is not meant to be publicly accessible over the web, it's supposed to be run on LAN,
because it can connect to other servers through SSH in order to execute Bash commands.</p>",
        
        "<h2>Quick start</h2>
<ol>
    <li>Edit the settings in <code>engine/settings.inc</code>, and adjust <code>.htaccess</code> file for the rewrite rules if needed.</li>
    <li>Install (once) DB Tables in ". l( 'system/setup/install', 'system/setup/install' ) ."</li>
    <li>Import (once) defaults in ". l( 'system/setup/load-configuration', 'system/setup/load-configuration' ) ."</li>
    <li>Pick a path to start with &ndash; e.g. for ". l( "/test", "test" ) ." you need to create <code>context/test.inc</code>.<br>In this file, set these 2 variables, like : <code>\$head_page_title = 'My page title';</code> and <code>\$content .= \"Hello world\";</code>.<br>
        Once this is done, ". l( "have a look at the result", 'test' ) .".</li>
    <li>You may provide \"Navbar\" items in <code>context/global.inc</code></li>
    <li><i class=\"icon-hand-up icon-large\"></i> have a look at the snippets in the <code>System</code> menu.</li>
</ol>"
    ),
));

?>
<hr>
<h2>Simplified Schema</h2>
<p>Showing briefly the database, and generally, the underlying organization of this "mini-engine".</p>
<div style="text-align:center"><img src="<?php print $base_path; ?>theme/img/mini-engine-schema-v07.png" alt="schéma des entités telles qu'attendues par Drupal" /></div>
