<?php

//      Engine settings
$base_path = '/mini-engine/bashpad-0.0.2/';
$extensions[ 'bashpad' ][ 'enabled' ] = true;
$extensions[ 'db' ][ 'settings' ][ 'default' ][ 'dbname' ] = 'mydb';
$extensions[ 'db' ][ 'settings' ][ 'default' ][ 'user' ] = 'myuser';
$extensions[ 'db' ][ 'settings' ][ 'default' ][ 'pass' ] = 'mypass';

//      Content settings
$page_title = "Welcome";
$metadata[ 'site_name' ] = "Bash Pad";
$metadata[ 'description' ] = "Shell commands store & 'launch pad'";

//      Debug
$enable_debug = true;
//$debug[ 'extensions' ] = 1;
//$debug[ 'routing' ] = 1;
//$debug[ 'tpl' ] = 1;
//$debug[ 'req' ] = 1;
//$debug[ 'exec_time' ] = 1;

