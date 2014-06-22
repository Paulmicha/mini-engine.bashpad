<?php

/**
 *  @file
 *  Commands - Launch
 *  @path /batch/123/render
 */

//      Init
$head_page_title = "<i class='icon-rocket'></i> Batch $batch_title";

//      Flush previously set html page content
$content = "";

//      Render
$content .= batch_render( $arg1 );


