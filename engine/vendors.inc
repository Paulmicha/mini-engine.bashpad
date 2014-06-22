<?php

/**
 *  @file
 *  Mini engine hub file - External libs
 */

if ( $enable_debug ) {
    require "vendor/krumo/class.krumo.php";
}

if ( $enable_simple_htmldom || $enable_form || $enable_form_multigroup ) {
    require "vendor/simple_html_dom.php";
}

if ( $enable_ssh )
{
    require 'vendor/phpseclib0.3.5/Net/SSH2.php';
    require 'vendor/phpseclib0.3.5/Net/SFTP.php';
    require 'vendor/phpseclib0.3.5/Crypt/RSA.php';
}


