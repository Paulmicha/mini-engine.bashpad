<?php

/**
 *  @file
 *  Database API
 */


/**
 *  Singleton PDO instance
 */
function db_pdo()
{
    //      @evol 16:50 21/04/2012 - easily possible to work with several databases, with :
    //$GLOBALS[ 'db_active' ]
    static $db;
    if ( !$db )
    {
        extract( db_get_credentials());
        $db = new PDO( 'mysql:host=localhost;dbname='. $db_name, $db_user, $db_pass );
        $db->setAttribute( PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION );
        $db->setAttribute( PDO::MYSQL_ATTR_USE_BUFFERED_QUERY, true );
        $db->exec( 'SET CHARACTER SET utf8' );
    }
    return $db;
}



/**
 *  Raw PDO query execution
 *  @see db_insert()
 */
function db_query( $query, $parameters = array(), $parameters_are_named = false )
{
    $db = db_pdo();
    try
    {
        if ( !empty( $parameters ))
        {
            if ( $parameters_are_named )
            {
                //      Example with named parameters :
                //$sql = 'SELECT name, colour, calories FROM fruit WHERE calories < :calories AND colour = :colour';
                //$sth = $dbh->prepare($sql, array(PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY));
                //$sth->execute(array(':calories' => 150, ':colour' => 'red'));
                //$red = $sth->fetchAll();
                
                $exec_query = $db->prepare( $query, array( PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY ));
                $exec_query->execute( $parameters );
            }
            else
            {
                //      Example with question mark parameters :
                //$sth = $dbh->prepare('SELECT name, colour, calories FROM fruit WHERE calories < ? AND colour = ?');
                //$sth->execute(array(150, 'red'));
                //$red = $sth->fetchAll();
                //$sth->execute(array(175, 'yellow'));
                //$yellow = $sth->fetchAll();
                
                $exec_query = $db->prepare( $query );
                $exec_query->execute( $parameters );
            }
        }
        else
        {
            //      Example without (security concerns)
            //$sql = "SELECT name, colour, calories FROM fruit WHERE calories < $calories AND colour = $colour";
            //$result = $connection->query($query);
            //$row = $result->fetch(PDO::FETCH_ASSOC);
            
            $db->exec( $query );
        }
    }
    catch( Exception $e )
    {
        //      Allow debug error
        if ( $GLOBALS[ 'debug' ] && $GLOBALS[ 'debug' ][ 'error' ])
            print '<pre><code class="sql">'. $e ."\n"."Query : ". $query .'</code></pre>';
        
        return false;
    }
    
    //      Execution happened normally if we got this far
    return true;
}



/**
 *  Basic PDO helper
 *  Get just the first key from the first result
 */
function db_popfetch( $query, $parameters = array(), $parameters_are_named = false )
{
    $res = db_fetch( $query, $parameters, $parameters_are_named );
    if ( $res && is_array( $res ))
        return array_shift( $res );
    else
        return false;
}



/**
 *  Basic PDO helper
 *  Get just the first result
 */
function db_fetch( $query, $parameters = array(), $parameters_are_named = false )
{
    $db = db_pdo();
    try
    {
        if ( $parameters_are_named )
        {
            $exec_query = $db->prepare( $query, array( PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY ));
            $exec_query->execute( $parameters );
        }
        else
        {
            $exec_query = $db->prepare( $query );
            $exec_query->execute( $parameters );
        }
        return $exec_query->fetch( PDO::FETCH_ASSOC );
    }
    catch( Exception $e )
    {
        //      Allow debug error
        if ( $GLOBALS[ 'debug' ] && $GLOBALS[ 'debug' ][ 'error' ])
            print '<pre><code class="error">'. $e ."\n"."Query : ". $query .'</code></pre>';
    }
    return false;
}



/**
 *  Basic PDO fetchall helper
 *  @return Array: structure [ num_row ][ 'nom_champ' ] = valeur
 */
function db_fetchall( $query, $parameters = array(), $parameters_are_named = false )
{
    $db = db_pdo();
    try
    {
        if ( $parameters_are_named )
        {
            $exec_query = $db->prepare( $query, array( PDO::ATTR_CURSOR => PDO::CURSOR_FWDONLY ));
            $exec_query->execute( $parameters );
        }
        else
        {
            $exec_query = $db->prepare( $query );
            $exec_query->execute( $parameters );
        }
        return $exec_query->fetchAll( PDO::FETCH_ASSOC );
    }
    catch( Exception $e )
    {
        //      Allow debug error
        if ( $GLOBALS[ 'debug' ] && $GLOBALS[ 'debug' ][ 'error' ])
            print '<pre><code class="error">'. $e ."\n"."Query : ". $query .'</code></pre>';
    }
    return false;
}



/**
 *  Pager queries helper
 *  
 *  @return Array
 *  @example
 *      array( 'pager' => array( 'pager_id' => 1, 'total_results_nb' => 123 ), 'results' => array( 0 => array( 'field_name' => "value" )))
 */
function pager_query( $query, $parameters = array(), $options = array())
{
    //      Overridable defaults
    $options += array(
        'current_page' => 0,
        'nb_per_page' => 25,
        //'pager_id' => null,
        //'count_query' => null,
        //'count_query_parameters' => null,
        'count_query_parameters_are_named' => false,
        'parameters_are_named' => false,
    );
    extract( $options );
    
    //      Failsafe
    //      Can't have a LIMIT statement in the query already
    if ( strpos( $query, "LIMIT" ))
        return false;
    
    //      Static pager id
    static $pager_id;
    if ( !$pager_id )
        $pager_id = 1;
    else
        $pager_id++;
    
    //      Construct a count query if none was given
    //      @see https://api.drupal.org/api/drupal/includes%21pager.inc/function/pager_query/6
    if ( !isset( $count_query )) {
        $count_query = preg_replace( array( '/SELECT.*?FROM /As', '/ORDER BY .*/' ), array( 'SELECT COUNT(*) as count FROM ', '' ), $query );
    }
    
    //      GET-based pagination
    if ( !empty( $_GET[ "p$pager_id" ]) && is_numeric( $_GET[ "p$pager_id" ]))
        $current_page = $_GET[ "p$pager_id" ];
    
    //      Auto limit
    $limit_start = $current_page * $nb_per_page;
    $query .= " LIMIT $limit_start,$nb_per_page";
    
    //      Fetch paged results & Total nb of results
    $results = db_fetchall( $query, $parameters, $parameters_are_named );
    $total_results_nb = db_popfetch( $count_query, $parameters, $parameters_are_named );
    
    //      When we have more results than the total number of results,
    //      we don't need a pager
    if ( $nb_per_page >= $total_results_nb )
        return array( 'results' => $results );
    
    //      Paged results & pager details
    else
        return array(
            'results' => $results,
            'pager' => array(
                'pager_id' => $pager_id,
                'nb_per_page' => $nb_per_page,
                'current_page' => $current_page,
                'total_results_nb' => $total_results_nb,
            )
        );
}



/**
 *  Basic PDO insert helper
 *  Inserts a new row in $table with $values = array( 'field_name' => "field value" )
 *  @return (int) last inserted id, or false when insertion failed
 */
function db_insert( $table, $values )
{
    if ( !empty( $table ) && !empty( $values ))
    {
        $placeholders = array();
        $sanitized_keys = array();
        $pdo_parameters = array();
        $sanitized_table = check_plain( $table );
        
        foreach( $values as $key => $val )
        {
            $placeholders[] = '?';
            $pdo_parameters[] = $val;
            $sanitized_keys[] = check_plain( $key );
        }
        
        $db = db_pdo();
        if ( db_query( "INSERT INTO `$sanitized_table` ( ". join( ', ', $sanitized_keys ) ." ) VALUES ( ". join( ', ', $placeholders ) ." );", $pdo_parameters ))
            return $db->lastInsertId();
    }
    return false;
}



/**
 *  Basic PDO - INSERT IGNORE helper
 */
function db_insert_ignore( $table, $values = array())
{
    if ( !empty( $table ) && !empty( $values ))
    {
        $placeholders = array();
        $sanitized_keys = array();
        $pdo_parameters = array();
        $sanitized_table = check_plain( $table );
        
        foreach( $values as $key => $val )
        {
            $placeholders[] = '?';
            $pdo_parameters[] = $val;
            $sanitized_keys[] = check_plain( $key );
        }
        
        $db = db_pdo();
        if ( db_query( "INSERT IGNORE INTO `$sanitized_table` ( ". join( ', ', $sanitized_keys ) ." ) VALUES ( ". join( ', ', $placeholders ) ." );", $pdo_parameters ))
            return $db->lastInsertId();
    }
    return false;
}



/**
 *  Basic PDO - DELETE helper
 */
function db_delete( $table, $options = array())
{
    extract( $options );
    
    //     Option 'primary_key_name' is optional when naming convention used is :
    if ( empty( $primary_key_name ))
        $primary_key_name = 'id_'. $table;
    
    if ( !empty( $table ) && ( !empty( $id ) || ( !empty( $where ) && !empty( $where_params ))))
    {
        $pdo_parameters = array();
        $sanitized_table = check_plain( $table );
        
        //      Prepare WHERE
        //      Option to override 'where'
        if ( !empty( $where ) && !empty( $where_params ))
            $pdo_parameters = $where_params;
        else
        {
            $where = "`$primary_key_name` = ?";
            $pdo_parameters[] = $id;
        }
        
        return db_query( "DELETE FROM `$sanitized_table` WHERE $where;", $pdo_parameters );
    }
    return false;
}



/**
 *  Basic PDO update helper
 *  Updates a row in $table, with $options = array(
 *      'id' => PRIMARY_KEY_VALUE (int),
 *      'primary_key_name' => 'id_table_uncommon',
 *  )
 *  @return (int) updated row id, or false when update failed
 */
function db_update( $table, $values, $options = array())
{
    extract( $options );
    
    //     Option 'primary_key_name' is optional when naming convention used is :
    if ( empty( $primary_key_name ))
        $primary_key_name = 'id_'. $table;
    
    //      @evol option to override 'where' (thus making $id optional)
    if ( !empty( $table ) && !empty( $values ) && ( !empty( $id ) || ( !empty( $where ) && !empty( $where_params ))))
    {
        $pdo_parameters = array();
        $sanitized_table = check_plain( $table );
        
        //      Prepare SET
        $set = '';
        $set_arr = array();
        foreach( $values as $key => $val )
        {
            $set_arr[] = check_plain( $key ) ." = ?";
            $pdo_parameters[] = $val;
        }
        $set = join( ',', $set_arr );
        
        //      Prepare WHERE
        //      Option to override 'where'
        if ( !empty( $where ) && !empty( $where_params ))
            $pdo_parameters = $where_params;
        else
        {
            $where = "`$primary_key_name` = ?";
            $pdo_parameters[] = $id;
        }
        
        $feedback_update = db_query( "UPDATE `$sanitized_table` SET $set WHERE $where;", $pdo_parameters );
        
        if ( $feedback_update )
            return $id;
    }
    return false;
}



/**
 *  DB Helper : Insert or Update
 *  Only inserts row if entry does not already exist, otherwise updates the existing row
 *  and returns the existing row's 'id_primary_key'
 *  @return id_primary_key
 */
function insert_or_update( $table, $values, $options = array())
{
    extract( $options );
    
    //      Required
    if ( empty( $table ) || empty( $values ))
        return false;
    
    //      Default unique field name is 'title'
    if ( empty( $unique_field_name ))
        $unique_field_name = 'title';
    
    //     Option 'primary_key_name' is optional when naming convention used is :
    if ( empty( $primary_key_name ))
        $primary_key_name = 'id_'. $table;
    
    //      Option 'unique_field_value' is optional if it exists inside 'values'
    if ( empty( $unique_field_value ))
        $unique_field_value = @$values[ $unique_field_name ];
    
    //      Can't continue without a unique field value
    //      unless we have a custom duplicate check query
    if ( !@$detect_duplicate_query && empty( $values[ $unique_field_name ]))
        return false;
    
    $unique_field_value = addcslashes( $unique_field_value, '"' );
    
    //      Looking for existing records
    //      Make the query to detect existing entries overridable
    if ( !@$detect_duplicate_query )
    {
        if ( is_numeric( $unique_field_value ))
            $check = db_fetchall( "SELECT `$primary_key_name` FROM `$table` WHERE `$unique_field_name` = $unique_field_value" );
        else
            $check = db_fetchall( 'SELECT `'. $primary_key_name .'` FROM `'. $table .'` WHERE `'. $unique_field_name .'` LIKE "'. $unique_field_value .'"' );
    }
    else {
        $check = db_fetchall( $detect_duplicate_query );
    }
    
    //      debug
    //print '<pre>detect_duplicate_query :</pre>';
    //krumo( $detect_duplicate_query );
    //print '<pre>check :</pre>';
    //krumo( $check );
    
    //      Update
    if ( !empty( $check ))
    {
        $id = @$check[ 0 ][ $primary_key_name ];
        if ( !empty( $id ))
        {
            return db_update( $table, $values, array(
                'id' => $id,
                'primary_key_name' => $primary_key_name,
            ));
        }
    }
    
    //      Insert
    else
        return db_insert( $table, $values );
    
    return false;
}



/**
 *  DB Helper : Insert without making duplicates
 *  Only inserts row if entry does not already exist, otherwise returns the existing row's 'id_primary_key'
 *
 *  @return id_primary_key
 */
function insert_if_no_existing_record( $table, $values, $options = array())
{
    extract( $options );
    
    //      ex $options :
    //array(
    //    'unique_field_name' => 'channel_name',
    //)
    
    //      Default unique field name : 'title'
    if ( empty( $table ) || empty( $values ))
        return false;
    if ( empty( $unique_field_name ))
        $unique_field_name = 'title';
    
    //     Option 'primary_key_name' is optional when naming convention used is :
    if ( empty( $primary_key_name ))
        $primary_key_name = 'id_'. $table;
    
    //      Option 'unique_field_value' is optional if it exists inside 'values'
    if ( empty( $unique_field_value ))
        $unique_field_value = @$values[ $unique_field_name ];
    
    //      Can't continue without a unique field value
    //      unless we have a custom duplicate check query
    if ( !@$detect_duplicate_query && empty( $values[ $unique_field_name ]))
        return false;
    
    $unique_field_value = addcslashes( $unique_field_value, '"' );
    
    //      Looking for existing records
    //      Make the query to detect existing entries overridable
    if ( !@$detect_duplicate_query )
    {
        if ( is_numeric( $unique_field_value ))
            $check = db_fetchall( "SELECT `$primary_key_name` FROM `$table` WHERE `$unique_field_name` = $unique_field_value" );
        else
            $check = db_fetchall( 'SELECT `'. $primary_key_name .'` FROM `'. $table .'` WHERE `'. $unique_field_name .'` LIKE "'. $unique_field_value .'"' );
    }
    else {
        $check = db_fetchall( $detect_duplicate_query );
    }
    
    //      debug
    //print '<pre>detect_duplicate_query :</pre>';
    //krumo( $detect_duplicate_query );
    //print '<pre>primary_key_name : '. $primary_key_name .'</pre>';
    //print '<pre>check :</pre>';
    //krumo( $check );
    
    if ( !empty( $check[ 0 ][ $primary_key_name ]))
        $id = $check[ 0 ][ $primary_key_name ];
    else
        $id = db_insert( $table, $values );
    
    return $id;
}



/**
 *  DB Validation helper
 */
function mandatory_values_check( $values, $mandatory_fields )
{
    if ( empty( $values ) || empty( $mandatory_fields ))
        return false;
    foreach( $mandatory_fields as $field )
    {
        if ( !isset( $values[ $field ]))
            return false;
        if ( !is_numeric( $values[ $field ]) && empty( $values[ $field ]))
            return false;
    }
    return true;
}


