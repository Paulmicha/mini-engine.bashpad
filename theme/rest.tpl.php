<?php

/**
 *  @file
 *  REST raw base template
 */

//      Bounce
if ( $unauthorized ) {
    print "<p class='error'> Error: unauthorized </p>";
}
else {
    print $content_rendered;
    print $spaces[ 'page_bottom' ];
}

