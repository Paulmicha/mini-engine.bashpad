<?php

/**
 *  @file
 *  Test : db helpers
 *  @path /system/test/db
 */

//      Init
$head_page_title = "DB API";
$tests = array();

//      db_query() - CREATE TABLE 'test_db_creation'
$tests[] = array(
    'desc' => "Testing <code>db_query()</code> - CREATE TABLE 'test_db_creation'",
    'result' => db_query( "CREATE TABLE IF NOT EXISTS `test_db_creation` (`id_test_db_creation` BIGINT unsigned NOT NULL AUTO_INCREMENT, `title` VARCHAR(255) COLLATE utf8_unicode_ci, `datestamp` BIGINT unsigned, PRIMARY KEY (`id_test_db_creation`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;" ),
);

//      db_insert()
$datestamp = datestamp();
$values = array(
    'title' => "I am 1 test ù£\$àç&à'èé!",
    'datestamp' => $datestamp,
);
$inserted_id = db_insert( 'test_db_creation', $values );
$tests[] = array(
    'desc' => "Testing <code>db_insert()</code>",
    'result' => ( $inserted_id == 1 ),
    //'debug' => $inserted_id,
);

//      insert_or_update()
$insert_or_update_id = insert_or_update( 'test_db_creation', $values );
$tests[] = array(
    'desc' => "Testing <code>insert_or_update()</code>",
    'result' => ( $inserted_id == $insert_or_update_id ),
    //'debug' => $insert_or_update_id,
);

//      insert_or_update() with param 'unique_field_name'
$insert_or_update_id = insert_or_update( 'test_db_creation', $values, array(
    'unique_field_name' => 'datestamp'
));
$tests[] = array(
    'desc' => "Testing <code>insert_or_update()</code> with param '<code>unique_field_name</code>'",
    'result' => ( $inserted_id == $insert_or_update_id ),
    //'debug' => $insert_or_update_id,
);

//      insert_if_no_existing_record()
$insert_if_no_existing_record_id = insert_if_no_existing_record( 'test_db_creation', $values );
$tests[] = array(
    'desc' => "Testing <code>insert_if_no_existing_record()</code>",
    'result' => ( $inserted_id == $insert_if_no_existing_record_id ),
    //'debug' => $insert_if_no_existing_record_id,
);

//      insert_if_no_existing_record() with param 'unique_field_name'
$insert_if_no_existing_record_id = insert_if_no_existing_record( 'test_db_creation', $values, array(
    'unique_field_name' => 'datestamp'
));
$tests[] = array(
    'desc' => "Testing <code>insert_if_no_existing_record()</code> with param '<code>unique_field_name</code>'",
    'result' => ( $inserted_id == $insert_if_no_existing_record_id ),
    //'debug' => $insert_if_no_existing_record_id,
);

//      db_update()
$values = array(
    'title' => "Renommer titre",
);
$updated_id = db_update( 'test_db_creation', $values, array( 'id' => $inserted_id ));
$tests[] = array(
    'desc' => "Testing <code>db_update()</code>",
    'result' => ( $updated_id == $inserted_id ),
    //'debug' => $updated_id,
);

//      db_fetchall()
$db_fetchall = db_fetchall( 'SELECT * FROM test_db_creation' );
$tests[] = array(
    'desc' => "Testing <code>db_fetchall()</code>",
    'result' => (
        is_array( $db_fetchall ) &&
        !empty( $db_fetchall ) &&
        $db_fetchall[ 0 ][ 'title' ] == $values[ 'title' ]
    ),
    //'debug' => print_r( $db_fetchall, 1 ),
);

//      db_fetch()
$db_fetch = db_fetch( 'SELECT * FROM test_db_creation' );
$tests[] = array(
    'desc' => "Testing <code>db_fetch()</code>",
    'result' => (
        is_array( $db_fetch ) &&
        !empty( $db_fetch ) &&
        $db_fetch[ 'title' ] == $values[ 'title' ]
    ),
    //'debug' => print_r( $db_fetch, 1 ),
);

//      db_popfetch()
$db_popfetch = db_popfetch( 'SELECT * FROM test_db_creation' );
$tests[] = array(
    'desc' => "Testing <code>db_popfetch()</code>",
    'result' => (
        !empty( $db_popfetch ) &&
        $db_popfetch == reset( $db_fetch )
    ),
    //'debug' => print_r( $db_popfetch, 1 ),
    //'debug' => reset( $db_fetch ),
);

//      db_delete()
$db_delete = db_delete( 'test_db_creation', array( 'id' => $db_popfetch ));
$tests[] = array(
    'desc' => "Testing <code>db_delete()</code>",
    'result' => ( $db_delete == $db_popfetch ),
    //'debug' => $db_delete,
);


//      db_query() - TRUNCATE TABLE 'test_db_creation'
$tests[] = array(
    'desc' => "Testing <code>db_query()</code> - TRUNCATE TABLE 'test_db_creation'",
    'result' => db_query( "TRUNCATE TABLE `test_db_creation`;" ),
);

//      db_query() - DROP TABLE 'test_db_creation'
$tests[] = array(
    'desc' => "Testing <code>db_query()</code> - DROP TABLE 'test_db_creation'",
    'result' => db_query( "DROP TABLE `test_db_creation`;" ),
);



//      Write results
//$content .= _render_tests( $tests );


//      Explanations
$snippets_html = '';

$snippets_html .= '';
$snippets_html .= "";

//      db_query
$snippets_html .= '<h4>1. "Raw" query</h4>';
$code = <<<'EOT'
//      This returns Boolean : success flag
$success_flag = db_query( "CREATE TABLE IF NOT EXISTS `test_db_creation` (
    `id_test_db_creation` BIGINT unsigned NOT NULL AUTO_INCREMENT,
    `title` VARCHAR(255) COLLATE utf8_unicode_ci,
    `datestamp` BIGINT unsigned,
    PRIMARY KEY (`id_test_db_creation`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;" );
EOT;
$snippets_html .= view_source( $code );

//      db_insert
$snippets_html .= '<h4>2. Inserts</h4>';
$code = <<<'EOT'
//      This returns Integer : last inserted id
$inserted_id = db_insert( 'test_db_creation', array(
    'title' => "I am 1 test ù£\$àç&à'èé!",
    'datestamp' => datestamp(),
));
EOT;
$snippets_html .= view_source( $code );

//      insert_or_update
$snippets_html .= '<h4>3. Insert or Update</h4>';
$code = <<<'EOT'
//      This returns Integer : inserted or updated id
//      Note : $values is the array from previous snippet (2nd param)
$insert_or_update_id = insert_or_update( 'test_db_creation', $values );
EOT;
$snippets_html .= view_source( $code );

//      insert_or_update
$snippets_html .= '<h4>4. Insert or Update - with param "unique_field_name"</h4>';
$code = <<<'EOT'
//      This returns Integer : inserted or updated id
$insert_or_update_id = insert_or_update( 'test_db_creation', $values, array(
    'unique_field_name' => 'datestamp'
));
EOT;
$snippets_html .= view_source( $code );

//      insert_or_update
$snippets_html .= '<h4>5. Insert if no existing record</h4>';
$code = <<<'EOT'
//      This returns Integer : inserted or existing id
$insert_if_no_existing_record_id = insert_if_no_existing_record( 'test_db_creation', $values );
EOT;
$snippets_html .= view_source( $code );

//      insert_or_update
$snippets_html .= '<h4>6. Insert if no existing record - with param "unique_field_name"</h4>';
$code = <<<'EOT'
//      This returns Integer : inserted or existing id
$insert_if_no_existing_record_id = insert_if_no_existing_record( 'test_db_creation', $values, array(
    'unique_field_name' => 'datestamp'
));
EOT;
$snippets_html .= view_source( $code );

//      db_update
$snippets_html .= '<h4>7. Update</h4>';
$code = <<<'EOT'
//      This returns Integer : updated row id
$updated_id = db_update( 'test_db_creation', array( 'title' => "Renommer titre" ), array( 'id' => $inserted_id ));
EOT;
$snippets_html .= view_source( $code );

//      db_fetchall
$snippets_html .= '<h4>8. Fetch all</h4>';
$code = <<<'EOT'
//      This returns Array : all rows, as an associative array (with column names as keys)
$db_fetchall = db_fetchall( 'SELECT * FROM test_db_creation' );
EOT;
$snippets_html .= view_source( $code );
$snippets_html .= '<p>Résultat : </p>';
$snippets_html .= view_source( '//      Pour info, on debug avec :
print kpr( $db_fetchall );' );
$snippets_html .= kpr( $db_fetchall );

//      db_fetch
$snippets_html .= '<h4>9. Fetch</h4>';
$code = <<<'EOT'
//      This returns Array : only 1st row, as an associative array (with column names as keys)
$db_fetch = db_fetch( 'SELECT * FROM test_db_creation' );
EOT;
$snippets_html .= view_source( $code );
$snippets_html .= '<p>Résultat : </p>';
$snippets_html .= kpr( $db_fetch );

//      db_popfetch
$snippets_html .= '<h4>10. Pop Fetch</h4>';
$code = <<<'EOT'
//      This returns (mixed) : 1st column value from 1st row
$db_popfetch = db_popfetch( 'SELECT * FROM test_db_creation' );
EOT;
$snippets_html .= view_source( $code );
$snippets_html .= '<p>Résultat : </p>';
$snippets_html .= kpr( $db_popfetch );

//      db_delete
$snippets_html .= '<h4>11. Delete</h4>';
$code = <<<'EOT'
//      This returns Integer : the deleted row id
$db_delete = db_delete( 'test_db_creation', array( 'id' => $db_popfetch ));
EOT;
$snippets_html .= view_source( $code );





//      Render
$content .= grid( array(
    array(  "<h2>Snippets</h2><p>(Code used for each test)</p>", "<h2>Tests</h2>" ),
    array( $snippets_html, _render_tests( $tests )),
));







