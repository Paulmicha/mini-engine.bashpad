<?php

/**
 *  @file
 *  Test : Data table
 *  @path /system/test/datatable
 */

//      Init
$head_page_title = "Testing data table";

//      Explications
$content .= grid( array(
    array(  "<h2>Code</h2>", "<h2>Result</h2>" ),
));

//      Code + preview
$code = <<<'EOT'
//      Auto-magically preview the content of ANY table in the database
print view_db_table( 'cache' );
EOT;
$content .= grid( array(
    array( view_source( $code ), view_db_table( 'cache' )),
));



