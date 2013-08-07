<?php

/**
 *  @file
 *  Test : Simple views
 *  @path /system/test/simple-views
 */

//      Init
$head_page_title = "Testing simple views";

//      Explications
$content .= grid( array(
    array(  "<h2>Code</h2>", "<h2>Résultat</h2>" ),
));

//      Code + preview
$code = <<<'EOT'
//      Auto-magically preview the content of ANY table in the database
print view_db_table( 'cache' );
EOT;
$content .= grid( array(
    array( view_source( $code ), view_db_table( 'cache' )),
));



