<?php

/**
 *  @file
 *  Entity API - Storage
 */


/**
 *  Storage "driver" SQL
 *  Converts entity metadata array into SQL "schema"
 *  -> executable SQL query to create corresponding tables & fields
 *  
 *  Assumptions :
 *      all entites tables are named like their $type
 *      always have a primary key field "id_$type" BIGINT unsigned NOT NULL AUTO_INCREMENT
 */
function entity_sql_storage( $type, $metadata = null )
{
    $schema_sql = '';
    
    //      Failsafe
    $type = str_clean( $type, '_' );
    if ( empty( $metadata ) || !is_array( $metadata )) {
        return $schema_sql;
    }
    
    //      Begin query
    $schema_sql .= "CREATE TABLE IF NOT EXISTS `$type` (";
    $schema_sql .= PHP_EOL;
    $schema_sql .= "`id_$type` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT";
    
    //      Fields
    if ( !empty( $metadata[ 'fields' ]))
    {
        foreach( $metadata[ 'fields' ] as $field_name => $field_definition )
        {
            $field_name = str_clean( $field_name, '_' );
            
            $schema_sql .= ",";
            $schema_sql .= PHP_EOL;
            $schema_sql .= "`$field_name` ";
            $schema_sql .= sql_driver_field( $field_definition );
        }
    }
    
    //      Primary key
    $schema_sql .= ",";
    $schema_sql .= PHP_EOL;
    $schema_sql .= "PRIMARY KEY (`id_$type`)";
    
    //      Keys
    if ( !empty( $metadata[ 'keys' ]))
    {
        foreach( $metadata[ 'keys' ] as $key )
        {
            $schema_sql .= ",";
            $schema_sql .= PHP_EOL;
            $schema_sql .= "KEY `$key` (`$key`)";
        }
    }
    
    //      Finish query
    $schema_sql .= PHP_EOL;
    $schema_sql .= ") ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;";
    $schema_sql .= PHP_EOL;
    
    
    //      Relations
    //      e.g. many to many (has_tag)
    if ( !empty( $metadata[ 'relations' ]))
    {
        foreach( $metadata[ 'relations' ] as $relation_name => $relation_definition )
        {
            //      Overridable defaults
            $relation_definition += array(
                'keys' => array(
                    'id',
                    "id_$type",
                ),
            );
            
            //      Adding default fields
            if ( empty( $relation_definition[ 'fields' ])) {
                $relation_definition[ 'fields' ] = array();
            }
            $relation_definition[ 'fields' ] += array(
                'id' => array(
                    'type' => 'integer',
                    'size' => 'big',
                    'unsigned' => true,
                ),
                "id_$type" => array(
                    'type' => 'integer',
                    'size' => 'big',
                    'unsigned' => true,
                ),
                'db_table' => array(
                    'type' => 'string',
                    'length' => 128,
                ),
            );
            
            //      Recursive call for additional relation table
            $schema_sql .= PHP_EOL;
            $schema_sql .= entity_sql_storage( $relation_name, $relation_definition );
        }
    }
    
    return $schema_sql;
}



/**
 *  Convert field definition array into SQL
 */
function sql_driver_field( $field_definition )
{
    $schema_sql = '';
    
    //      debug
    //krumo( $field_definition );
    
    //      Failsafe
    if ( empty( $field_definition ) || !is_array( $field_definition )) {
        return $schema_sql;
    }
    
    switch( $field_definition[ 'type' ])
    {
        case 'int' :
        case 'integer' :
            
            if ( !empty( $field_definition[ 'size' ]))
            {
                switch( $field_definition[ 'size' ])
                {
                    case "big" :
                        $schema_sql .= 'BIGINT';
                        break;
                    case "tiny" :
                    case "mini" :
                    case "small" :
                        $schema_sql .= 'TINYINT';
                        break;
                    default :
                        $schema_sql .= 'INT';
                        break;
                }
                
                /*
                if ( $field_definition[ 'size' ] == "big" ) {
                    $schema_sql .= 'BIGINT';
                }
                else if ( $field_definition[ 'size' ] == "tiny" ||
                    $field_definition[ 'size' ] == "mini" ||
                    $field_definition[ 'size' ] == "small" ) {
                    $schema_sql .= 'TINYINT';
                }
                else {
                    $schema_sql .= 'INT';
                }
                */
                
                //      Length when size is specified
                if ( !empty( $field_definition[ 'length' ])) {
                    $schema_sql .= '('. $field_definition[ 'length' ] .')';
                }
            }
            
            //      If we only have length, we could guess the size to use
            else if ( !empty( $field_definition[ 'length' ]))
            {
                //      @evol 03:40 21/04/2014 - check the limits & take "unsigned" into account
                if ( $field_definition[ 'length' ] > 15 ) {
                    $schema_sql .= 'BIGINT';
                }
                else if ( $field_definition[ 'length' ] > 3 ) {
                    $schema_sql .= 'INT';
                }
                else {
                    $schema_sql .= 'TINYINT';
                }
                
                $schema_sql .= '('. $field_definition[ 'length' ] .')';
            }
            
            //      Default
            else {
                $schema_sql .= 'INT';
            }
            
            //      Unsigned
            if ( !empty( $field_definition[ 'unsigned' ])) {
                $schema_sql .= ' UNSIGNED';
            }
            
            break;
        
        case 'str' :
        case 'txt' :
        case 'text' :
        case 'string' :
        case 'varchar' :
            
            //      Length
            if ( !empty( $field_definition[ 'length' ]))
            {
                if ( is_string( $field_definition[ 'length' ]))
                {
                    switch( $field_definition[ 'length' ])
                    {
                        case "big" :
                        case "longtext" :
                        case "long-text" :
                        case "long_text" :
                            $schema_sql .= 'LONGTEXT';
                            break;
                        
                        default :
                        case "text" :
                        case "normal" :
                            $schema_sql .= 'TEXT';
                            break;
                    }
                    
                    /*
                    if ( $field_definition[ 'length' ] == "big" ||
                        $field_definition[ 'length' ] == "longtext" ||
                        $field_definition[ 'length' ] == "long-text" ||
                        $field_definition[ 'length' ] == "long_text" ||
                        $field_definition[ 'length' ] == "long" ) {
                        $schema_sql .= 'LONGTEXT';
                    }
                    else {
                        $schema_sql .= 'TEXT';
                    }
                    */
                }
                else
                {
                    if ( $field_definition[ 'length' ] > 255 ) {
                        $schema_sql .= 'TEXT';
                    }
                    else {
                        $schema_sql .= 'VARCHAR('. $field_definition[ 'length' ] .')';
                    }
                }
            }
            
            //      Defaults to TEXT
            else {
                $schema_sql .= 'TEXT';
                
            }
            
            //      Use UTF-8 case insensitive (if no other encoding is specified)
            if ( empty( $field_definition[ 'encoding' ])) {
                $schema_sql .= ' COLLATE utf8_unicode_ci';
            }
            else {
                $schema_sql .= ' COLLATE '. $field_definition[ 'encoding' ];
            }
            
            break;
        
        case 'bin' :
        case 'binary' :
            
            if ( !empty( $field_definition[ 'size' ]))
            {
                if ( $field_definition[ 'size' ] == 'big' ||
                    $field_definition[ 'size' ] == 'long' ||
                    $field_definition[ 'size' ] == 'large' ) {
                    $schema_sql .= 'LONGBLOB';
                }
                else {
                    $schema_sql .= 'BLOB';
                }
            }
            else {
                $schema_sql .= 'BLOB';
            }
            
            break;
    }
    
    //      Mandatory
    if ( !empty( $field_definition[ 'not_null' ]) || !empty( $field_definition[ 'mandatory' ])) {
        $schema_sql .= ' NOT NULL';
    }
    
    //      Default value
    if ( !empty( $field_definition[ 'default' ])) {
        $schema_sql .= " DEFAULT '". $field_definition[ 'default' ] ."'";
    }
    
    //      debug
    //krumo( '$schema_sql' );
    //krumo( $schema_sql );
    
    return $schema_sql;
}



/**
 *  Build query to drop SQL tables corresponding to an entity
 */
function entity_sql_destroy( $type, $metadata = null )
{
    $destroy_sql = '';
    
    //      Failsafe
    $type = str_clean( $type, '_' );
    if ( empty( $metadata ) || !is_array( $metadata )) {
        return $destroy_sql;
    }
    
    //      Drop table for entity
    $destroy_sql .= "DROP TABLE IF EXISTS `$type`;";
    
    //      Relations
    //      e.g. many to many (has_tag)
    if ( !empty( $metadata[ 'relations' ]))
    {
        foreach( $metadata[ 'relations' ] as $relation_name => $relation_definition )
        {
            $destroy_sql .= PHP_EOL;
            $destroy_sql .= "DROP TABLE IF EXISTS `$relation_name`;";
        }
    }
    
    return $destroy_sql;
}


