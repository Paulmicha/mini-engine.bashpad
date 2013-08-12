<?php

/**
 *  @file
 *  Browse Tags
 *  
 *  @path system/tags
 *  @path system/tags/123
 *  @path system/tags/123/batch
 *  @path system/tags/123/command
 *  @path system/tags/123/environment
 *  @path system/tags/123/folder
 *  @path system/tags/123/thread
 */

//      Init
$head_page_title = "Browse Tags";



//-------------------------------------------------------------------------------------------
//      Local menus


$arg2 = check_plain( arg( 2 ));
$arg3 = check_plain( arg( 3 ));

$tab_items = array(
    array( "<i class='icon-tags'></i>&nbsp;Browse all tags", 'system/tags' ),
);
$subtab_items = array();
$subtab_tabs = array();

if ( is_numeric( $arg2 ))
{
    $tag_title = db_popfetch( "SELECT `title` FROM `tag` WHERE `id_tag` = ?", array( $arg2 ));
    $tab_items[] = array( "<i class='icon-tag'></i>&nbsp;$tag_title", 'system/tags/'. $arg2, 'path_is_in_base_route' );
    
    //      Count total number of "entities" associated with this term
    $nb = db_popfetch( "SELECT COUNT(`id_has_tag`) FROM `has_tag` WHERE `id_tag` = ?", array( $arg2 ));
    $subtab_items[] = array( "All <small>($nb)</small>", 'system/tags/'. $arg2 );
    
    //      Fetch all types of "entities" tagged with this term & render them as sub-tabs (pills)
    $types = db_fetchall( "SELECT DISTINCT(`db_table`) FROM `has_tag` WHERE `id_tag` = ?", array( $arg2 ));
    if ( !empty( $types ))
        foreach( $types as $type )
        {
            //      Count total number of "entities" for this type associated with this term
            $nb = db_popfetch( "SELECT COUNT(`id_has_tag`) FROM `has_tag` WHERE `id_tag` = ? AND `db_table` = ?", array( $arg2, $type[ 'db_table' ]));
            $subtab_items[] = array( ucfirst( $type[ 'db_table' ]) ." <small>($nb)</small>", 'system/tags/'. $arg2 .'/'. $type[ 'db_table' ]);
        }
}

$content .= nav_tabs( $tab_items );

if ( !empty( $subtab_items ))
    $content .= nav_pills( $subtab_items );



//-------------------------------------------------------------------------------------------
//      Dispatch


//      Browse all tags
if ( empty( $arg2 ))
{
    $content .= "<ul>";
    $content .= recursive_tag_tree_builder();
    $content .= "</ul>";
}

//      Single tag browsing
else if ( is_numeric( $arg2 ))
{
    //      Unfiltered
    if ( empty( $arg3 ))
    {
        $head_page_title = "All entities tagged '$tag_title'";
        
        if ( !empty( $types ))
            foreach( $types as $type )
            {
                $table = $type[ 'db_table' ];
                $query = "SELECT `$table`.*
                    FROM `$table`
                    INNER JOIN `has_tag` ON `$table`.id_$table = `has_tag`.id
                        AND `has_tag`.id_tag = ?
                    GROUP BY `$table`.id_$table
                    ORDER BY `$table`.id_$table DESC";
                
                $content .= "<h3>". ucfirst( $table ) ."</h3>";
                $content .= render_pager_query_table( $query, array( $arg2 ), array(
                    'preprocess_results' => '_preprocess_entities',
                    'table_options' => array(
                        'header' => array(
                            "ID",
                            "Title",
                            "Description",
                            "Data",
                            "Tags",
                        ),
                    ),
                ));
            }
    }
    
    //      Filter by entity type
    else
    {
        $head_page_title = ucfirst( $arg3 ) ." entities tagged '$tag_title'";
        
        $query = "SELECT `$arg3`.*
            FROM `$arg3`
            INNER JOIN `has_tag` ON `$arg3`.id_$arg3 = `has_tag`.id
                AND `has_tag`.id_tag = ?
            GROUP BY `$arg3`.id_$arg3
            ORDER BY `$arg3`.id_$arg3 DESC";
        
        $content .= render_pager_query_table( $query, array( $arg2 ), array(
            'preprocess_results' => '_preprocess_entities',
            'table_options' => array(
                'header' => array(
                    "ID",
                    "Title",
                    "Description",
                    "Data",
                    "Tags",
                ),
            ),
        ));
    }
}



/**
 *  Recursively construct tree of tags
 */
function recursive_tag_tree_builder( $id_tag_parent = 0 )
{
    $html = '';
    
    //      Level 0 (no parent)
    if ( empty( $id_tag_parent ))
    {
        $lv0_tags = db_fetchall(
            "SELECT `tag`.id_tag as id_tag, `tag`.title as title
            FROM `tag`
            LEFT JOIN `has_tag` ON `tag`.id_tag = `has_tag`.id AND `has_tag`.db_table LIKE 'tag'
            WHERE `has_tag`.id IS NULL
            ORDER BY `tag`.title ASC"
        );
        
        foreach( $lv0_tags as $tag )
        {
            $html .= '<li>';
            $html .= l( "<i class='icon-tag'></i>&nbsp;". $tag[ 'title' ], 'system/tags/'. $tag[ 'id_tag' ]);
            $html .= '</small>';
            
            //      Children
            $children_str = recursive_tag_tree_builder( $tag[ 'id_tag' ]);
            if ( !empty( $children_str ))
            {
                $html .= '<ul>';
                $html .= $children_str;
                $html .= '</ul>';
            }
            
            $html .= '</li>';
        }
    }
    
    //      Levels 1+
    else
    {
        $tags_by_parent = db_fetchall(
            "SELECT `tag`.id_tag as id_tag, `tag`.title as title
                FROM `tag`
                LEFT JOIN `has_tag` ON `tag`.id_tag = `has_tag`.id AND `has_tag`.db_table LIKE 'tag'
                WHERE `has_tag`.id_tag = ?
                ORDER BY `tag`.title ASC",
            array( $id_tag_parent )
        );
        
        foreach( $tags_by_parent as $tag )
        {
            $html .= '<li>';
            $html .= l( "<i class='icon-tag'></i>&nbsp;". $tag[ 'title' ], 'system/tags/'. $tag[ 'id_tag' ]);
            
            //      Children
            $children_str = recursive_tag_tree_builder( $tag[ 'id_tag' ]);
            if ( !empty( $children_str ))
            {
                $html .= '<ul>';
                $html .= $children_str;
                $html .= '</ul>';
            }
            
            $html .= '</li>';
        }
    }
    
    return $html;
}



/**
 *  Preprocess pager query results
 *  -> make table lines from query results
 */
function _preprocess_entities( $results )
{
    $lines_arr = array();
    
    foreach( $results as $r )
    {
        //      Let's assume that the first key is always gonna be like "id_DBTABLENAME"
        $keys = array_keys( $r );
        $first_key = reset( $keys );
        $table = substr( $first_key, 3 );
        
        if ( !empty( $r[ 'data' ]))
            $r[ 'data' ] = safe_unserialize( $r[ 'data' ]);
        
        //      Assemble columns
        $lines_arr[] = array(
            
            //      ID
            $r[ 'id_'. $table ],
            
            //      Description
            $r[ 'title' ],
            
            //      Description
            $r[ 'description' ],
            
            //      Data
            kpr( $r[ 'data' ]),
            
            //      Tags
            view_tags( get_tags( 'batch', $r[ 'id_'. $table ])),
        );
    }
    
    return $lines_arr;
}


