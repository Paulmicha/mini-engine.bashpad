<?php

/**
 *  @file
 *  Mini-engin - tags api
 */


/**
 *  Fetch first tag ID matching provided case-insensitive title
 *  @param String $title : case-insensitive title
 *  @return Integer : 'id_tag'
 */
function get_tag_by_title( $title )
{
    $title = strtolower( $title );
    return db_popfetch( "SELECT `id_tag` FROM `tag` WHERE LOWER(`title`) LIKE ?", array( $title ));
}



/**
 *  Save tags in a hierarchical array
 *  @example engine/core/config/tags.inc
 */
function save_hierarchical_tags( $tags = array(), $parent_tag_id = null )
{
    if ( !empty( $tags ))
        foreach( $tags as $key => $tag )
        {
            //      If it's a parent tag with children, the title is the key (and the value most likely an array)
            if ( is_string( $key ) && !empty( $key ))
            {
                $id_tag = insert_if_no_existing_record( 'tag', array( 'title' => trim( $key )));
                
                //      Set parent relation
                if ( !empty( $id_tag ) && !empty( $parent_tag_id ))
                    //insert_if_no_existing_record(
                    insert_or_update(
                        'has_tag',
                        array(
                            'id' => $id_tag,
                            'id_tag' => $parent_tag_id,
                            'db_table' => 'tag',
                        ),
                        array(
                            'detect_duplicate_query' => "SELECT `id_tag` FROM `has_tag` WHERE `db_table` LIKE 'tag' AND `id` = $id_tag AND `id_tag` = $parent_tag_id",
                        )
                    );
            }
            
            //      Recursive call if children is an array
            if ( is_array( $tag ))
            {
                if ( !empty( $id_tag ))
                    save_hierarchical_tags( $tag, $id_tag );
                else
                    save_hierarchical_tags( $tag, $parent_tag_id );
            }
            
            //      Normal tag (or leaf)
            else if ( !empty( $tag ))
            {
                //$id_tag = insert_if_no_existing_record( 'tag', array( 'title' => trim( $tag )));
                $id_tag = insert_or_update( 'tag', array( 'title' => trim( $tag )));
                
                if ( !empty( $id_tag ) && !empty( $parent_tag_id ))
                    //insert_if_no_existing_record(
                    insert_or_update(
                        'has_tag',
                        array(
                            'id' => $id_tag,
                            'id_tag' => $parent_tag_id,
                            'db_table' => 'tag',
                        ),
                        array(
                            'detect_duplicate_query' => "SELECT `id_tag` FROM `has_tag` WHERE `db_table` LIKE 'tag' AND `id` = $id_tag AND `id_tag` = $parent_tag_id",
                        )
                    );
            }
        }
}



/**
 *  Fetch all tags associated with an entity, by DB table and by ID.
 *  @param String $table : db table
 *  @param Integer $id : entity primary key value
 *  @return Array : list of tags (raw db result)
 */
function view_tags( $tags = array(), $options = array())
{
    $return = '';
    if ( !empty( $tags ))
        foreach( $tags as $id_tag )
        {
            $tag_data = db_fetch( "SELECT * FROM `tag` WHERE `id_tag` = ?", array( $id_tag ));
            $return .= '<span class="badge badge-inverse">';
            $return .= $tag_data[ 'title' ];
            
            //      Tag id
            //$return .= '&nbsp;<small class="muted">';
            //$return .= $tag_data[ 'id_tag' ];
            //$return .= '</small>';
            
            $return .= '</span> ';
        }
    return $return;
}



/**
 *  Fetch all tags associated with an entity, by DB table and by ID.
 *  @param String $table : db table
 *  @param Integer $id : entity primary key value
 *  @return Array : list of tags (raw db result)
 */
function get_tags( $table, $id, $options = array())
{
    $id_tag_arr = array();
    $tags_data = db_fetchall( "SELECT `id_tag` FROM `has_tag` WHERE `db_table` LIKE ? AND `id` = ?", array( $table, $id ));
    
    if ( !empty( $tags_data ))
        foreach( $tags_data as $t )
            $id_tag_arr[] = $t[ 'id_tag' ];
    
    return $id_tag_arr;
}



/**
 *  Handles association and/or creation + association of tags,
 *  and also removing tags.
 *  
 *  @param Array $tags : list of tags to associate or create & associate,
 *      by DB table and by ID.
 *  @return Boolean : success flag (@todo - validation)
 *  
 *  @example in server.inc
 *  @see _environment_form_validate()
 *  @see _environment_form_submit()
 */
function set_tags( $tags = array())
{
    $return = true;
    
    if ( !empty( $tags ))
        foreach( $tags as $table => $data )
            foreach( $data as $id => $actions )
            {
                //      Failsafe
                if ( !is_numeric( $id ))
                    continue;
                
                //      Check if previously associated tags were removed
                $previous_tags = get_tags( $table, $id );
                if ( !empty( $previous_tags ))
                    foreach( $previous_tags as $previous_tag_id )
                        if (( !empty( $actions[ 'tags' ]) && !in_array( $previous_tag_id, $actions[ 'tags' ])) || empty( $actions[ 'tags' ]))
                            db_query( "DELETE FROM `has_tag` WHERE `db_table` LIKE ? AND `id` = ? AND `id_tag` = ?", array( $table, $id, $previous_tag_id ));
                
                //      Make new tags, and add their ids to $actions[ 'tags' ]
                //      in order to associate them all at once
                //      Note : we might encouter the situation where the same tag is created several times in a single post
                //      @see server.inc
                if ( !empty( $actions[ 'new_tags' ]))
                    foreach( $actions[ 'new_tags' ] as $new_tag_title )
                        if ( !empty( $new_tag_title ))
                            $actions[ 'tags' ][] = insert_if_no_existing_record( 'tag', array( 'title' => trim( $new_tag_title )));
                
                //      This is an array of existing tags ids,
                //      all we have to do is make the "link" via the 'has_tag' table
                $table = check_plain( $table );
                if ( !empty( $actions[ 'tags' ]))
                    foreach( $actions[ 'tags' ] as $id_tag )
                        if ( !empty( $id ) && !empty( $id_tag ) && !empty( $table ))
                            insert_if_no_existing_record(
                                'has_tag',
                                array(
                                    'id' => $id,
                                    'id_tag' => $id_tag,
                                    'db_table' => $table,
                                ),
                                array(
                                    'detect_duplicate_query' => "SELECT `id_tag` FROM `has_tag` WHERE `db_table` LIKE '$table' AND `id` = $id AND `id_tag` = $id_tag",
                                )
                            );
            }
    
    return $return;
}



/**
 *  Get an array to form select options in forms
 */
function tags_select_options( $options = array())
{
    $tags = array();
    
    //      Overridable defaults
    $options += array(
        'max' => 2550,
        'filter' => '', // @evol 2013/07/22 02:06:34
    );
    extract( $options );
    
    //      Make it optional -> first item in list must be empty
    //      multiselect : no need
    //$tags[ '' ] = "---";
    
    //      Fetch all tags
    $query = "SELECT * FROM `tag` ORDER BY `title` ASC";
    if ( $max > 0 )
        $query .= " LIMIT 0,$max";
    
    //      One option per tag
    $results = db_fetchall( $query );
    if ( !empty( $results ))
        foreach( $results as $r )
            $tags[ 'id-'. $r[ 'id_tag' ]] = $r[ 'title' ];
    
    return $tags;
}



/**
 *  Helper to convert all submitted values
 *  of a multiselect field with values from tags_select_options()
 */
function clean_values_tags_select_options( $submitted_options )
{
    if ( !empty( $submitted_options ))
        foreach( $submitted_options as $i => $opt )
            $submitted_options[ $i ] = str_replace( 'id-', '', $opt );
    return $submitted_options;
}


