<?php

/**
 *  @file
 *  Cache explorer
 *  @path /system/cache-explorer
 */

//      Init
$head_page_title = "Cache entries";

$content .= view_db_table( 'cache' );

