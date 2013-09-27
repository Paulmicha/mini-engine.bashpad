<?php

/**
 *  @file
 *  Simple "views" API
 */


/**
 *  Render pager queries as table
 */
function render_pager_query_table( $query, $parameters = array(), $options = array())
{
    //      Overridable defaults
    $options += array(
        'pager_query_options' => array(),
        'table_options' => array(
            
            //      Keep this comment for option syntax reference
            /*
            'header' => array(
                
                //      Polymorphisme : soit string, soit array
                'col-1' => "Titre table Head",
                'col-2' => array(
                    'label' => "Titre table Head",
                    'attributes' => array(
                        'colspan' => "2",
                        'class' => "custom-class",
                    ),
                ),
                
                //      $key optionnel
                "Titre simple",
            ),
            */
        ),
        
        //      Optional preprocess function for results
        //      (to be able to control the rendering of each table line from DB result)
        //      Note : must return an array, as expected by table() first param
        'preprocess_results' => '',
    );
    extract( $options );
    
    //      Fetch pager results
    $data = pager_query( $query, $parameters, $pager_query_options );
    
    if ( empty( $data[ 'results' ]) && empty( $data[ 'pager' ]))
        return alert( 'Nothing to display.' );
    else
    {
        $html = '';
        $lines_arr = array();
        
        //      Do we have a valid preprocess function,
        //      to take care of the rendering of each table lines from paged DB results ?
        if ( !empty( $preprocess_results ) && function_exists( $preprocess_results ))
            $lines_arr = $preprocess_results( $data[ 'results' ]);
        
        //      Default rendering for table lines
        else
            foreach( $data[ 'results' ] as $i => $r )
                foreach( $r as $key => $col )
                    $lines_arr[ $i ][ $key ] = $col;
        
        //      Failsafe
        if ( empty( $lines_arr ))
            return alert( 'No lines to display.' );
        
        //      Table HTML
        $html .= table( $lines_arr, $table_options );
        
        //      Pager HTML
        if ( !empty( $data[ 'pager' ]))
            $html .= pager( $data[ 'pager' ]);
        
        return $html;
    }
}



/**
 *  Get first 500 lines (default value) from any table,
 *  rendered in a data table widget
 *  
 *  Note : assumes all columns of type "blob" to be encoded with safe_serialize()
 *  
 *  @return String : rendered html
 */
function view_db_table( $table, $options = array())
{
    //      Overridable defaults
    $options += array(
        'max_lines' => 500,
        'fields' => array(),  //  Must be structured like the result of db_table_get_columns()
        'query' => '',
        'query_parameters' => array(),
        'query_parameters_are_named' => false,
        'prevent_auto_unserialize' => false,
        'serialized_fields' => array(),  //  for previewing serialized columns with krumo
    );
    extract( $options );
    
    //      Failsafe
    if ( empty( $table ))
        return '';
    
    //      Fields
    $field_list = array();
    if ( empty( $fields ))
        $fields = db_table_get_columns( $table );
    if ( !empty( $fields ))
        foreach( $fields as $i => $field )
            $field_list[ $i ] = $field[ 'Field' ];
    
    //      Query
    $table = check_plain( $table );
    if ( empty( $query ))
        $query = "SELECT `". join( '`, `', $field_list ) ."` FROM `$table` LIMIT 0,$max_lines";
    
    //      Results
    $results = db_fetchall( $query, $query_parameters, $query_parameters_are_named );
    
    //      debug
    //krumo( $fields );
    //krumo( $query );
    //krumo( $results );
    //return kpr( $results );
    
    //      Empty
    if ( empty( $results ))
        return alert( "Table <code>$table</code> is empty." );
    
    //      Data table lines
    $lines = array();
    foreach( $results as $i => $r )
    {
        $j = 0;
        foreach( $r as $column => $value )
        {
            //      Automatic unserialization of columns of type "blob",
            //      handle columns specified as serialized
            if ( !$prevent_auto_unserialize && (
                $fields[ $j ][ 'Type' ] == 'blob' ||
                $fields[ $j ][ 'Type' ] == 'longblob' ||
                in_array( $column, $serialized_fields ))
            ) {
                $value = kpr( safe_unserialize( $value ));
            }
            
            $lines[ $i ][ $column ] = $value;
            $j++;
        }
    }
    
    //      Render data table
    return datatable( $lines, array( 'header' => $field_list ));
}



/**
 *  Fetch all column names from a table
 */
function db_table_get_columns( $table )
{
    $table = check_plain( $table );
    return db_fetchall( "SHOW COLUMNS FROM `$table`" );
}


