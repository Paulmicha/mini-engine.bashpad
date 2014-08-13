<?php

/**
 *  @file
 *  System UI helpers
 *  HTML theming helpers based on Bootstrap 2.3.2
 */


/**
 *  Navbar
 *  @param $items Array : nav items
 */
function navbar( $items, $options = array())
{
    //      Overridable defaults
    $options += array(
        'class' => 'nav-pills',
    );
    return navs( $items, $options );
}


/**
 *  Nav Pills
 *  @param $items Array : nav items
 */
function nav_pills( $items, $options = array())
{
    //      Overridable defaults
    $options += array(
        'class' => 'nav-pills',
    );
    return navs( $items, $options );
}



/**
 *  Nav Tabs
 *  @param $items Array : nav items
 */
function nav_tabs( $items, $options = array())
{
    //      Overridable defaults
    $options += array(
        'class' => 'nav-tabs',
    );
    return navs( $items, $options );
}



/**
 *  Navs
 *  @param $items Array : nav items
 */
function navs( $items, $options = array())
{
    //      Adding mandatory class 'nav'
    $options[ 'class' ] = 'nav '. $options[ 'class' ];
    $attributes = attributes( $options );
    
    //      Polymorphism : each item may be an array or a string (html of item)
    //      Arrays can be like :
    //          array( "Title", 'my/path' )
    //          array( "Title", 'my/path', 'path_is_in_base_route' )
    $items_html = '';
    foreach( $items as $item )
        if ( is_string( $item ))
            $items_html .= $item;
        else
        {
            $active_callback = 'path_is_active';
            
            if ( !empty( $item[ 2 ]))
                $active_callback = $item[ 2 ];
            
            $items_html .= menu_item( $item[ 0 ], $item[ 1 ], '', array( 'active_callback' => $active_callback ));
        }
    
    return "<ul$attributes>$items_html</ul>";
}



/**
 *  Definition lists
 *  @param $items Array : items with keys as "terms" (<dt>) of definition list
 */
function dli( $items, $options = array())
{
    //      Overridable defaults
    $options += array(
        'class' => 'dl-horizontal',
    );
    
    $return = '';
    $return .= '<dl'. attributes( $options ) .'>';
    foreach( $items as $term => $defintion )
    {
        $return .= "<dt>$term</dt>";
        $return .= "<dd>$defintion</dd>";
    }
    $return .= '</dl>';
    
    return $return;
}



/**
 *  Grids
 *  @see http://twitter.github.io/bootstrap/base-css.html#tables
 */
function grid( $lines, $options = array())
{
    $html = '';
    
    //      Overridable defaults
    $options += array(
        'total_cols' => 12,
    );
    extract( $options );
    
    if ( !empty( $lines ))
    {
        foreach( $lines as $i => $line )
        {
            //      Columns
            if ( !empty( $line ))
            {
                $cols = array();
                
                //      Polymorphisme pour le contenu des lignes :
                //          soit array avec 'attributes' et 'columns',
                //          soit simplement un array de strings pour le contenu des lignes
                if ( !empty( $line[ 'columns' ]))
                {
                    $cols = $line[ 'columns' ];
                    $line[ 'attributes' ] += array( 'class' => "row-fluid" );
                    $html .= "<div". attributes( $line[ 'attributes' ]) .">";
                }
                else
                {
                    $cols = $line;
                    $html .= '<div class="row-fluid">';
                }
                
                //      Defaults to equal-width columns
                $max_cols = count( $cols );
                $span_nb = floor( $total_cols / $max_cols );
                
                foreach( $cols as $j => $col )
                {
                    //      Polymorphisme pour le contenu des cols :
                    //          soit array avec 'attributes' et/ou 'content',
                    //          soit simplement une string
                    if ( is_array( $col ))
                    {
                        $col[ 'attributes' ] += array( 'class' => "span$span_nb" );
                        $html .= "<div". attributes( $col[ 'attributes' ]) .">";
                        $html .= $col[ 'content' ];
                        $html .= '</div>';
                    }
                    else
                    {
                        $html .= "<div". attributes( array( 'class' => "span$span_nb" )) .">";
                        $html .= $col;
                        $html .= '</div>';
                    }
                }
                
                $html .= '</div>';
            }
        }
    }
    
    return $html;
}



/**
 *  Data Table
 *  
 *  @extends table()
 *  @see http://datatables.net/
 *  @see http://datatables.net/blog/Twitter_Bootstrap_2
 */
function datatable( $lines, $options = array())
{
    //      Overridable defaults
    $options += array(
        'attributes' => array(
            'class' => "table table-hover table-bordered dataTable data-table-hook",
        ),
    );
    return table( $lines, $options );
}



/**
 *  Tables
 *  @see http://twitter.github.io/bootstrap/base-css.html#tables
 */
function table( $lines, $options = array())
{
    $html = '';
    
    //      Overridable defaults
    $options += array(
        
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
        
        'attributes' => array(
            'class' => "table table-hover",
        ),
        
        //      @evol 2013/06/13 21:27:09
        //      Option to prevent the following default behavior :
        //      to propagate header columns' class to lines columns
        //'prevent_header_class_propagation' => false,
        
        //      Same goes for inline styles
        //'prevent_header_style_propagation' => false,
    );
    extract( $options );
    
    //      C'est moche sans header
    if ( empty( $header ))
        $attributes[ 'class' ] .= ' table-bordered';
    
    $html .= '<table'. attributes( $attributes ) .'>';
    
    //      Table headers
    $cols_classes = array();
    if ( !empty( $header ))
    {
        $html .= '<thead>';
        $html .= '<tr>';
        foreach( $header as $id => $header_line )
        {
            if ( is_array( $header_line ))
            {
                $html .= "<th". attributes( $header_line[ 'attributes' ]) .">";
                $html .= $header_line[ 'label' ];
                $html .= '</th>';
            }
            else
            {
                $html .= "<th>";
                $html .= $header_line;
                $html .= '</th>';
            }
        }
        $html .= '</tr>';
        $html .= '</thead>';
    }
    
    //      Lines
    if ( !empty( $lines ))
    {
        $html .= '<tbody>';
        foreach( $lines as $i => $line )
        {
            //      Columns
            if ( !empty( $line ))
            {
                $cols = array();
                
                //      Polymorphisme pour le contenu des lignes :
                //          soit array avec 'attributes' et 'columns',
                //          soit simplement un array de strings pour le contenu des lignes
                if ( !empty( $line[ 'columns' ]))
                {
                    $html .= "<tr". attributes( $line[ 'attributes' ]) .">";
                    $cols = $line[ 'columns' ];
                }
                else
                {
                    $html .= "<tr>";
                    $cols = $line;
                }
                
                foreach( $cols as $j => $col )
                {
                    //      Polymorphisme pour le contenu des cols :
                    //          soit array avec 'attributes' et/ou 'content',
                    //          soit simplement une string
                    if ( is_array( $col ))
                    {
                        $html .= "<td". attributes( $col[ 'attributes' ]) .">";
                        $html .= $col[ 'content' ];
                        $html .= '</td>';
                    }
                    else
                    {
                        $html .= "<td>";
                        $html .= $col;
                        $html .= '</td>';
                    }
                }
                
                $html .= '</tr>';
            }
        }
        $html .= '</tbody>';
    }
    $html .= '</table>';
    
    return $html;
}



/**
 *  Simple print alert (Twitter Bootstrap style)
 *  $type can be 'info' || 'success' || 'warning' || 'error'
 */
function alert( $msg, $type = 'info', $options = array())
{
    extract( $options );
    $classes = 'alert-'. $type;
    if ( !empty( $extra_classes ))
        $classes .= ' '. $extra_classes;
    if ( empty( $no_icon ))
    {
        $correspondance_type_icon_arr = array(
            'info' => 'info-sign',
            'success' => 'ok',
            'warning' => 'warning-sign',
            'error' => 'remove',
        );
        $msg = '<i class="icon-'. $correspondance_type_icon_arr[ $type ] .'"></i>&nbsp;'. $msg;
    }
    return <<<EOD
<div class="alert $classes">
    <button type="button" class="close" data-dismiss="alert">&times;</button>
    $msg
</div>
EOD;

}



/**
 *  Progress bar
 */
function progress_bar( $percent, $options = array())
{
    //      Overridable defautls
    $options += array(
        'extra_classes' => "progress-striped active",
        'bar_extra_classes' => "",
        'display_percent' => true,
    );
    extract( $options );
    
    if ( $percent == 100 )
    {
        $bar_extra_classes .= 'bar-success';
        if ( !empty( $extra_classes ))
            $extra_classes = str_replace( 'progress-striped', '', $extra_classes );
    }
    
    $percent_txt = '';
    if ( $display_percent )
        $percent_txt = "<div style='text-align:center'>". round( $percent ) ."&nbsp;%</div>";
        //$percent_txt = round( $percent ) ."&nbsp;%";
        //$percent_txt = "<div style='float:left'>". round( $percent ) ."&nbsp;%</div>";
    
    return <<<EOD
$percent_txt
<div class="progress $extra_classes">
  <div class="bar $bar_extra_classes" style="width: $percent%;"></div>
</div>
EOD;

}



/**
 *  Get single menu item,
 *  handling 'active' class based on path
 */
function view_source( $code, $language = 'php' )
{
    $code = str_replace( '<', '&lt;', $code );
    $code = str_replace( '>', '&gt;', $code );
    return "<pre style='padding:0'><code class='$language'>$code</code></pre>";
}


