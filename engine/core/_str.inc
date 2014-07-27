<?php

/**
 *  @file
 *  String utilities
 */


/**
 *  Encodes special characters in a plain-text string for display as HTML.
 *  Also validates strings as UTF-8 to prevent cross site scripting attacks on Internet Explorer 6.
 *  @see http://api.drupal.org/api/drupal/includes%21bootstrap.inc/function/check_plain/7
 */
function check_plain( $str ) {
    return htmlspecialchars( $str, ENT_QUOTES, 'UTF-8' );
}



/**
 *  Problems encoding serialized strings with certain accents
 */
function safe_serialize( $data ) {
    return base64_encode( serialize( $data ));
}


/**
 *  Problems encoding serialized strings with certain accents
 */
function safe_unserialize( $data ) {
    return unserialize( base64_decode( $data ));
}



/**
 *  Make sure string is UTF-8
 */
function ensure_utf8( $str )
{
    if ( !mb_check_encoding( $str, 'UTF-8' )) {
        $str = mb_convert_encoding( $str, "UTF-8" );
    }
    
    return $str;
}



/**
 *  Enlève tous les caractères non-espace & convertit les accents
 *  @param  String $str: la chaine à convertir
 *  @param  [optional] String $separator: le séparateur qui remplace les accents
 *  @return String: la chaine convertie
 */
function str_clean( $str, $separator = '-', $options = array())
{
    //      debug
    //krumo::backtrace();
    
    //      Overridable defaults
    $options += array(
        'preserve_case' => false,
        'preserve_chars' => '',
    );
    extract( $options );
    
    $str = trim( $str );
    
    if ( !$preserve_case ) {
        $str = strtolower( $str );
    }
    
    $str = basic_transliteration( $str );
    $str = strip_line_breaks( $str, $separator );
    $str = preg_replace( '`[^a-zA-Z0-9\\'. $separator . $preserve_chars .']`', $separator, $str );
    $str = preg_replace( '`'. $separator .'+`', $separator, $str );
    
    return $str;
}



/**
 *  Translitération des accents les plus courants
 *  @see    http://fr.php.net/manual/fr/function.strtr.php
 */
function basic_transliteration( $str )
{
    $str = ensure_utf8( $str );
    return strtr( $str, array(
        'Š'=>'S', 'š'=>'s', 'Đ'=>'Dj', 'đ'=>'dj', 'Ž'=>'Z', 'ž'=>'z', 'Č'=>'C', 'č'=>'c', 'Ć'=>'C', 'ć'=>'c',
        'À'=>'A', 'Á'=>'A', 'Â'=>'A', 'Ã'=>'A', 'Ä'=>'A', 'Å'=>'A', 'Æ'=>'A', 'Ç'=>'C', 'È'=>'E', 'É'=>'E',
        'Ê'=>'E', 'Ë'=>'E', 'Ì'=>'I', 'Í'=>'I', 'Î'=>'I', 'Ï'=>'I', 'Ñ'=>'N', 'Ò'=>'O', 'Ó'=>'O', 'Ô'=>'O',
        'Õ'=>'O', 'Ö'=>'O', 'Ø'=>'O', 'Ù'=>'U', 'Ú'=>'U', 'Û'=>'U', 'Ü'=>'U', 'Ý'=>'Y', 'Þ'=>'B', 'ß'=>'Ss',
        'à'=>'a', 'á'=>'a', 'â'=>'a', 'ã'=>'a', 'ä'=>'a', 'å'=>'a', 'æ'=>'a', 'ç'=>'c', 'è'=>'e', 'é'=>'e',
        'ê'=>'e', 'ë'=>'e', 'ì'=>'i', 'í'=>'i', 'î'=>'i', 'ï'=>'i', 'ð'=>'o', 'ñ'=>'n', 'ò'=>'o', 'ó'=>'o',
        'ô'=>'o', 'õ'=>'o', 'ö'=>'o', 'ø'=>'o', 'ù'=>'u', 'ú'=>'u', 'û'=>'u', 'ý'=>'y', 'ý'=>'y', 'þ'=>'b',
        'ÿ'=>'y', 'Ŕ'=>'R', 'ŕ'=>'r'
    ));
}



/**
 *  Enlever les sauts à la ligne
 *  @param  String $str: la chaine à convertir
 *  @return String: la chaine convertie
 */
function strip_line_breaks( $str, $replace_break = ' ' )
{
	$chars = array("
", "\n", "\r", "chr(13)",  "\t", "\0", "\x0B" );
	return str_replace( $chars, $replace_break, $str );
}



/**
 *  Random password generator
 *  @see    http://api.drupal.org/api/drupal/modules--user--user.module/function/user_password/6
 */
function user_password( $length = 10 )
{
    $allowable_characters = 'abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ23456789';
    $len = strlen( $allowable_characters ) - 1;
    $pass = '';
    for ( $i = 0; $i < $length; $i++ ) {
        $pass .= $allowable_characters[ mt_rand( 0, $len )];
    }
    return $pass;
}



/**
 *  Try to close html tags in truncated html strings
 *  @see http://stackoverflow.com/questions/1193500/php-truncate-html-ignoring-tags
 *  @todo cleanup
 */
function repair_truncated_html_tags( $html, $maxLength = 0 )
{
    $return = '';
    $printedLength = 0;
    $position = 0;
    $tags = array();
    if ( !$maxLength ) {
        $maxLength = strlen( $html ) + 1;
    }
    while ($printedLength < $maxLength && preg_match('{</?([a-z]+)[^>]*>|&#?[a-zA-Z0-9]+;}', $html, $match, PREG_OFFSET_CAPTURE, $position))
    {
        list($tag, $tagPosition) = $match[0];

        // Print text leading up to the tag.
        $str = substr($html, $position, $tagPosition - $position);
        if ($printedLength + strlen($str) > $maxLength)
        {
            $return .= substr($str, 0, $maxLength - $printedLength);
            $printedLength = $maxLength;
            break;
        }

        $return .= $str;
        $printedLength += strlen($str);

        if ($tag[0] == '&')
        {
            // Handle the entity.
            $return .= $tag;
            $printedLength++;
        }
        else
        {
            // Handle the tag.
            $tagName = $match[1][0];
            if ($tag[1] == '/')
            {
                // This is a closing tag.

                $openingTag = array_pop($tags);
                //assert($openingTag == $tagName); // check that tags are properly nested.
                
                //      on ne laisse le tag que s'il était ouvert.
                if ( $openingTag == $tagName ) {
                    $return .= $tag;
                }
            }
            else if ($tag[strlen($tag) - 2] == '/')
            {
                // Self-closing tag.
                $return .= $tag;
            }
            else
            {
                // Opening tag.
                $return .= $tag;
                $tags[] = $tagName;
            }
        }

        // Continue after the tag.
        $position = $tagPosition + strlen($tag);
    }

    // Print any remaining text.
    if ($printedLength < $maxLength && $position < strlen($html))
        $return .= substr($html, $position, $maxLength - $printedLength);

    // Close any open tags.
    while (!empty($tags))
        $return .= sprintf('</%s>', array_pop($tags));
        //printf('</%s>', array_pop($tags));
    
    return $return;
}


