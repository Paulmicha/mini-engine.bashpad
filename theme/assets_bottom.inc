<?php

/**
 *  @file
 *  Template Fragment - Javascript at the bottom of HTML page, before </body>
 *  (used both in page.tpl.php and iframe.tml.php)
 */

?>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
    <script>window.jQuery || document.write('<script src="<?php print $base_path; ?>theme/js/vendor/jquery-1.9.1.min.js"><\/script>')</script>
    <script src="<?php print $base_path; ?>theme/js/vendor/bootstrap.min.js"></script>
    <script src="<?php print $base_path; ?>theme/js/vendor/jquery.dataTables.min.js"></script>
    <script src="<?php print $base_path; ?>theme/js/vendor/DT_bootstrap.js"></script>
    <script src="<?php print $base_path; ?>theme/js/vendor/highlight.pack.js"></script>
    <script src="<?php print $base_path; ?>theme/js/vendor/chosen.jquery.min.js"></script>
    <script src="<?php print $base_path; ?>theme/js/main.js"></script>
