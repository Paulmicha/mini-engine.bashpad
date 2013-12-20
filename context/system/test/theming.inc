<?php

/**
 *  @file
 *  Test : theming
 *  @path /system/test/theming
 */

//      Init
$head_page_title = "Test theming";



//------------------------------------------------------------
//      Navs


$content .= '<hr />';
$content .= '<h2>Navs</h2>';

$code = <<<'EOT'
//      Tabs
//      Note : "active item" handling is automatic
$content .= nav_tabs( array(
    array( "Section Landing Page", 'system/test/theming' ),
    array( "Create", 'system/test/crud/add' ),
    array( "Read", 'system/test/crud/123' ),
    array( "Update", 'system/test/crud/123/edit' ),
    array( "Delete", 'system/test/crud/123/delete' ),
));
EOT;
$content .= grid( array(
    array(
        nav_tabs( array(
            array( "Section Landing Page", 'system/test/theming' ),
            array( "Create", 'system/test/crud/add' ),
            array( "Read", 'system/test/crud/123' ),
            array( "Update", 'system/test/crud/123/edit' ),
            array( "Delete", 'system/test/crud/123/delete' ),
        )),
        view_source( $code ),
    )
));

$content .= '<hr />';
$code = <<<'EOT'
//      Pills
$content .= nav_pills( array(
    array( "Section Landing Page", 'system/test/theming' ),
    array( "Create", 'system/test/crud/add' ),
    array( "Read", 'system/test/crud/123' ),
    array( "Update", 'system/test/crud/123/edit' ),
    array( "Delete", 'system/test/crud/123/delete' ),
));
EOT;
$content .= grid( array(
    array(
        nav_pills( array(
            array( "Section Landing Page", 'system/test/theming' ),
            array( "Create", 'system/test/crud/add' ),
            array( "Read", 'system/test/crud/123' ),
            array( "Update", 'system/test/crud/123/edit' ),
            array( "Delete", 'system/test/crud/123/delete' ),
        )),
        view_source( $code ),
    )
));



//------------------------------------------------------------
//      Alerts


$content .= '<hr />';
$content .= '<h2>Alert</h2>';

//      -> info
//$content .= alert( "I am an info <code>(no param)</code>" );
//      -> success
//$content .= alert( "I am a message of type :  <code>success</code>", 'success' );
//      -> warning
//$content .= alert( "I am a message of type :  <code>warning</code>", 'warning' );
//      -> error
//$content .= alert( "I am a message of type :  <code>error</code>", 'error' );

$content .= grid( array(
    array(
        alert( "I am an info <code>(no param)</code>" ) .
            alert( "I am a message of type :  <code>success</code>", 'success' ) .
            alert( "I am a message of type :  <code>warning</code>", 'warning' ) .
            alert( "I am a message of type :  <code>error</code>", 'error' ) ,
        view_source( '$content .= alert( "Je suis un message d\'info <code>(no param)</code>" );
$content .= alert( "I am a message of type :  <code>success</code>", \'success\' );
$content .= alert( "I am a message of type :  <code>warning</code>", \'warning\' );
$content .= alert( "I am a message of type :  <code>error</code>", \'error\' );' ),
    )
));



//------------------------------------------------------------
//      Grid


$content .= '<hr />';
$content .= '<h2>Grid</h2>';

$code = <<<'EOT'
$content .= grid( array(
    array( '<p class="well">Test 1.1</p>', '<p class="well">Test 1.2</p>', '<p class="well">Test 1.3</p>' ),
    array( '<p class="well">Test 2.1</p>', '<p class="well">Test 2.2</p>' ),
    array( '<p class="well">Test 3.1</p>', '<p class="well">Test 3.2</p>', '<p class="well">Test 3.3</p>', '<p class="well">Test 3.4</p>' ),
));
EOT;
$content .= view_source( $code );

$content .= grid( array(
    array( '<p class="well">Test 1.1</p>', '<p class="well">Test 1.2</p>', '<p class="well">Test 1.3</p>' ),
    array( '<p class="well">Test 2.1</p>', '<p class="well">Test 2.2</p>' ),
    array( '<p class="well">Test 3.1</p>', '<p class="well">Test 3.2</p>', '<p class="well">Test 3.3</p>', '<p class="well">Test 3.4</p>' ),
));

$code = <<<'EOT'
$content .= grid( array(
    array(
        array( 'content' => '<p class="well">Test 1.1</p>', 'attributes' => array( 'class' => "span7" )),
        array( 'content' => '<p class="well">Test 1.2</p>', 'attributes' => array( 'class' => "span5" )),
    ),
    array(
        '<p class="well">Test 3.1</p>',
        grid( array(
            array( '<p class="well">Nested Test 1.1</p>', '<p class="well">Nested Test 1.2</p>', '<p class="well">Nested Test 1.3</p>' )
        )),
    ),
));
EOT;
$content .= view_source( $code );

$content .= grid( array(
    array(
        array( 'content' => '<p class="well">Test 1.1</p>', 'attributes' => array( 'class' => "span7" )),
        array( 'content' => '<p class="well">Test 1.2</p>', 'attributes' => array( 'class' => "span5" )),
    ),
    array(
        '<p class="well">Test 2.1</p>',
        grid( array(
            array( '<p class="well">Nested Test 1.1</p>', '<p class="well">Nested Test 1.2</p>', '<p class="well">Nested Test 1.3</p>' )
        )),
    ),
));



//------------------------------------------------------------
//      Table


$content .= '<hr />';
$content .= '<h2>Table</h2>';

$code = <<<'EOT'
$content .= table( array(
    array( "Test 1.1", "Test 1.2", "Test 1.3", "Test 1.4" ),
    array( "Test 2.1", "Test 2.2", "Test 2.3", "Test 2.4" ),
    array( "Test 3.1", "Test 3.2", "Test 3.3", "Test 3.4" ),
));
EOT;
$content .= view_source( $code );

$content .= table( array(
    array( "Test 1.1", "Test 1.2", "Test 1.3", "Test 1.4" ),
    array( "Test 2.1", "Test 2.2", "Test 2.3", "Test 2.4" ),
    array( "Test 3.1", "Test 3.2", "Test 3.3", "Test 3.4" ),
));

$code = <<<'EOT'
$content .= table(
    array(
        array( "Test 1.1", "Test 1.2", "Test 1.3", "Test 1.4" ),
        array( "Test 2.1", array( 'content' => "Test 2.2", 'attributes' => array( 'style' => "font-weight:bold; color:limegreen;" )), "Test 2.3", "Test 2.4" ),
        array( "Test 3.1", "Test 3.2", "Test 3.3", "Test 3.4" ),
    ),
    array(
        'header' => array(
            'col-1' => "Title table Head 1",
            'col-2' => array(
                'label' => "Title table Head 2 (colspan 2)",
                'attributes' => array(
                    'colspan' => "2",
                    'class' => "custom-class",
                ),
            ),
            "Title simple",
        ),
    )
);
EOT;
$content .= view_source( $code );

$content .= table(
    array(
        array( "Test 1.1", "Test 1.2", "Test 1.3", "Test 1.4" ),
        array( "Test 2.1", array( 'content' => "Test 2.2", 'attributes' => array( 'style' => "font-weight:bold; color:limegreen;" )), "Test 2.3", "Test 2.4" ),
        array( "Test 3.1", "Test 3.2", "Test 3.3", "Test 3.4" ),
    ),
    array(
        'header' => array(
            'col-1' => "Title table Head 1",
            'col-2' => array(
                'label' => "Title table Head 2 (colspan 2)",
                'attributes' => array(
                    'colspan' => "2",
                    'class' => "custom-class",
                ),
            ),
            "Title simple",
        ),
    )
);




//------------------------------------------------------------
//      Data Table


$content .= '<hr />';
$content .= '<h2>Data table</h2>';

$code = <<<'EOT'
$dataTableRows_arr = array();
for ( $i = 0; $i < 255; $i++ )
    $dataTableRows_arr[] = array( "Test $i.1", "Test $i.2", "Test $i.3", "Test $i.4" );

$content .= datatable(
    $dataTableRows_arr,
    array(
        'header' => array(
            "Title simple 1",
            "Title simple 2",
            "Title simple 3",
            "Title simple 4",
        ),
    )
);
EOT;
$content .= view_source( $code );

$dataTableRows_arr = array();
for ( $i = 0; $i < 255; $i++ )
    $dataTableRows_arr[] = array( "Test $i.1", "Test $i.2", "Test $i.3", "Test $i.4" );

$content .= datatable(
    $dataTableRows_arr,
    array(
        'header' => array(
            "Title simple 1",
            "Title simple 2",
            "Title simple 3",
            "Title simple 4",
        ),
    )
);




//------------------------------------------------------------
//      Progress bars


$content .= '<hr />';
$content .= '<h2>Progress</h2>';

//$content .= progress_bar( 10 );
//$content .= progress_bar( 50 );
//$content .= progress_bar( 100 );

$content .= grid( array(
    array(
        progress_bar( 10 ) .
            progress_bar( 50 ) .
            progress_bar( 100 ) ,
        view_source( '$content .= progress_bar( 10 );
$content .= progress_bar( 50 );
$content .= progress_bar( 100 );' ),
    )
));





//------------------------------------------------------------


$content .= "";
$content .= '';





