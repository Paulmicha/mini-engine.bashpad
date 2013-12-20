<?php

/**
 *  @file
 *  Test : PhpSecLib
 *  @path /system/test/phpseclib
 */

//      Init
$head_page_title = "PhpSecLib";
$content .= "<p>@see <code>engine/core/api_seclib.inc</code></p>";
$content .= "<p><a href='http://phpseclib.sourceforge.net/index.html' target='_blank'><i class='icon-external-link'></i> Official website</a></p>";
$content .= "<p>@todo 2013/07/21 13:45:32</p>";


//      @todo 2013/07/21 13:45:32
$content .= "<hr>";
$code = <<<'EOT'
include('Net/SSH2.php');
include('Crypt/RSA.php');

define('NET_SSH2_LOGGING', 3);

$ssh = new Net_ssh2('10.1.75.200', 22);
$key = new Crypt_RSA();
$key->setPassword('zaq1@WSX');
$key->loadKey(file_get_contents('../../terminal/Key.ppk'));
if (!$ssh->login('username', $key)) {
    exit('Login Failed');
}
echo $ssh->exec('uptime');
EOT;
$result_html = 'Result :<br />';
//$result_html .= '<code>';
//$result_html .= '</code>';
$content .= grid( array(
    array( view_source( $code ), $result_html ),
));



