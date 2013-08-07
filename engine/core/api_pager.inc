<?php

/**
 *  @file
 *  Pager API
 */


/**
 *  Pagers
 *  Note: numbers are counted from 0, but for display, +1 is applied for humans
 *  @see http://twitter.github.io/bootstrap/components.html#pagination
 */
function pager( $pager = array())
{
    $html = '';
    
    //      Pager defaults
    $pager += array(
        'total_results_nb' => 0,
        'current_page' => 0,
        'nb_per_page' => 10,
        'pager_id' => 1,
        'nb_items_backwnforw' => 3,
    );
    extract( $pager );
    
    if ( $total_results_nb > $nb_per_page )
    {
        $html .= '<div class="pagination pagination-centered">';
        $html .= '<ul>';
        
        //      Calculate total number of pages
        $nb_pages = ceil( $total_results_nb / $nb_per_page ) - 1;
        $pager[ 'nb_pages' ] = $nb_pages;
        
        //      When < 10 pages : simpler pager
        if ( $nb_pages < 10 )
        {
            //      Previous
            $html .= _pager_prev( $pager );
            
            //      Up to 10 items
            for ( $i = 0; $i <= $nb_pages; $i++ )
                if ( $i == $current_page )
                    $html .= '<li class="active"><span>'. ( $i + 1 ) .'</span></li>';
                else
                    $html .= '<li><a href="'. pagination_url( $i, $pager_id ) .'">'. ( $i + 1 ) .'</a></li>';
            
            //      Next
            $html .= _pager_next( $pager );
        }
        
        //      10+ pages : more controls
        else
        {
            //      First page
            if ( $current_page == 0 )
                $html .= '<li class="active"><span><i class="icon-double-angle-left"></i></span></li>';
            else
                $html .= '<li><a href="'. pagination_url( 0, $pager_id ) .'"><i class="icon-double-angle-left"></i></a></li>';
            
            //      Previous
            $html .= _pager_prev( $pager );
            
            //      Ellipsis
            if ( $current_page > $nb_items_backwnforw )
                $html .= '<li><span>&hellip;</span></li>';
            
            //      Backward items
            $html .= _pager_backward_items( $pager );
            
            //      Current page
            $html .= '<li class="active"><span>'. ( $current_page + 1 ) .'</span></li>';
            
            //      Forward items
            $html .= _pager_forward_items( $pager );
            
            //      Ellipsis
            if ( $current_page + $nb_items_backwnforw < $nb_pages )
                $html .= '<li><span>&hellip;</span></li>';
            
            //      Next
            $html .= _pager_next( $pager );
        
            //      Last page
            if ( $current_page == $nb_pages )
                $html .= '<li class="disabled"><span>&raquo;</span></li>';
            else
                $html .= '<li><a href="'. pagination_url( $nb_pages, $pager_id ) .'">&raquo;</a></li>';
        }
        
        $html .= '</ul>';
        $html .= '</div>';
    }
    
    return $html;
}



/**
 *  Pager helper
 *  Will not affect other GET params, while updating the ones related to paging
 */
function pagination_url( $nb, $pager_id = 1 )
{
    //      We need current URL without any GET vars,
    //      so that we can re-build them afterwards
    $url = get_current_url( array( 'no_vars' => true ));
    
    //      Make a copy of GET vars' superglobal
    $url_vars = $_GET;
    
    //      Exclude from treatment the one reserved by rewriting system
    unset( $url_vars[ 'q' ]);
    
    //      Set paging var
    $url_vars[ "p$pager_id" ] = $nb;
    
    return $url .'?'. http_build_query( $url_vars );
}



/**
 *  Pager helper : previous item
 */
function _pager_prev( $pager )
{
    extract( $pager );
    if ( $current_page > 0 )
        return '<li><a href="'. pagination_url( $current_page - 1, $pager_id ) .'"><i class="icon-angle-left"></i></a></li>';
    else
        return '<li class="disabled"><span><i class="icon-angle-left"></i></span></li>';
}



/**
 *  Pager helper : next item
 */
function _pager_next( $pager )
{
    extract( $pager );
    if ( $current_page != $nb_pages )
        return '<li><a href="'. pagination_url( $current_page + 1, $pager_id ) .'"><i class="icon-angle-right"></i></a></li>';
    else
        return '<li class="disabled"><span><i class="icon-angle-right"></i></span></li>';
}



/**
 *  Pager helper : backward items
 */
function _pager_backward_items( $pager )
{
    extract( $pager );
    $html = '';
    $limit = 0;
    
    if ( $current_page - $nb_items_backwnforw < 1 )
        $limit = 1;
    else
        $limit = $current_page - $nb_items_backwnforw;
    
    for ( $i = $limit; $i <= $current_page - 1; $i++ )
        $html .= '<li><a href="'. pagination_url( $i + 1 ) .'"><i class="icon-angle-left"></i></a></li>';
    
    return $html;
}



/**
 *  Pager helper : forward items
 */
function _pager_forward_items( $pager )
{
    extract( $pager );
    $html = '';
    $limit = 0;
    
    if ( $current_page + $nb_items_backwnforw > $nb_pages )
        $limit = $nb_pages;
    else
        $limit = $current_page + $nb_items_backwnforw;
    
    for ( $i = $current_page + 1; $i <= $limit; $i++ )
        $html .= '<li><a href="'. pagination_url( $i + 1 ) .'"><i class="icon-angle-right"></i></a></li>';
    
    return $html;
}


