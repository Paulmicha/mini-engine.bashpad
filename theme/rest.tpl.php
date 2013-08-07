<?php

/**
 *  @file
 *  REST raw base template
 */

//      Bounce
if ( $unauthorized ) {
    print "<p class='error'> Erreur: votre session est invalide </p>";
}
else {
    print $content_markup;
    print $footer_extra_js;
}

