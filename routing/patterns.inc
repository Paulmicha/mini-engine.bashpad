<?php

/**
 *  @file
 *  Patterns - based on barebones
 *  @see http://barebones.paulrobertlloyd.com/_patterns.php
 *  
 *  @path patterns/*
 */

//      Look for theme files to generate content
$this_path_tpl = $theme_path .'/'. arg( 0 ) .'/'. arg( 1 );

if ( !file_exists( $this_path_tpl .'.tpl.php' )) {
    send_404();
}
else
{
    $files_arr = list_files( $this_path_tpl, array( 'mask' => '`\.html`' ));
    
    //      debug
    //krumo( $files_arr );
    
    foreach( $files_arr as $file )
    {
        $file_contents = file_get_contents( $file->fullpath );
        
        $content .= '<section class="pattern">';
        $content .= '<p>'. $file->fullpath .'</p>';
        $content .= $file_contents;
        $content .= '<details class="primer">';
		$content .= '<summary title="Show markup and usage">&#8226;&#8226;&#8226;</summary>';
		$content .= '<section>';
		$content .= '<textarea rows="10" cols="30" class="input code">'
            . htmlspecialchars( $file_contents )
            . '</textarea>';
		$content .= '<p class="caption"><strong>Usage:</strong> '
            . htmlspecialchars( @file_get_contents( $file->path .'/'. str_replace( '.html', '.txt', $file->filename )))
            . '</p>';
		$content .= '</section>';
        $content .= '</details><!--/.primer-->';
        $content .= '</section><!--/.pattern-->';
    }
}


//      Specific extra CSS
$head_extra_markup .= <<<EOT
    <style>
        .pattern {
            margin-top:4em;
        }
        details.primer {
            margin-top:2.5em;
            background-color:#e9e9e9;
            border-bottom:4px solid #e9e9e9;
            position:relative;
        }
        details.primer summary {
            font-size:1.5em;
            line-height:1;
            text-shadow:0 1px 0 #fff;
            background-color:#e9e9e9;
            border-radius:0.25em 0 0 0;
            padding:0.25em;
            overflow:hidden;
            position:absolute;
            right:0;
            top:-1.5em;
            }
            details.primer summary::-webkit-details-marker {
                display:none;
            }
        details.primer section {
            padding:1.5%;
            overflow:hidden;
        }
        details.primer p.caption {
            margin-left:0;
            margin-bottom:0;
        }
        @media screen and (min-width:40em) {
            details.primer textarea {
                width:58%;
                float:left;
            }
            details.primer p.caption {
                width:38%;
                float:right;
            }
        }
    </style>
EOT;

