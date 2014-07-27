<?php

/**
 *  @file
 *  Tokens API
 */


/**
 *  Get custom tokens
 *  
 *  @example [token_namespace]
 *  @example [token_namespace:qsdqsd123]
 *  
 *  input : cd [folder_svn] && svn add . --force && svn commit . -m "[commit_message]"
 *  input : cd [folder:project root]
 *  
 *  @return Array: array( 'token_namespace' => array( 0 => array( 'value' => '', 'match' => '[type_token]' )))
 *  @return Array: array( 'token_namespace' => array( 0 => array( 'value' => 'qsdqsd123', 'match' => '[type_token:qsdqsd123]' )))
 */
function simple_tokens_extract( $str )
{
    $return = array();
    if ( !empty( $str ))
    {
        $matches = array();
        
        //$feedback = preg_match_all( '`\[([^\]]+)\]`Ui', $str, $matches, PREG_SET_ORDER );
        
        //      Note : non-capturing parenthesis are written (?: )
        $feedback = preg_match_all( '`\[([^\:\]]+)(?::([^\]]+))?\]`Ui', $str, $matches, PREG_SET_ORDER );
        
        //      debug
        //krumo( '$matches' );
        //krumo( $matches );
        
        if ( $feedback && !empty( $matches ))
        {
            foreach( $matches as $i => $match )
            {
                $return[ $match[ 1 ]][] = array(
                    'value' => $match[ 2 ],
                    'match' => $match[ 0 ]
                );
            }
        }
    }
    return $return;
}


