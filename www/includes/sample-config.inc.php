<?php
  /* configuration settings */
  $main_title = "PTlink IRC Services";
  $default_lang = "en_us";
  // The base url for services
  $appurl = 'http://www.pt-link.net/services';  // No trailisn slash
  // path on the server for base directory
  $appdir = '/path/to/www_services/install';    // No trailing slash
  $dbhost     = "localhost";
  $dbuser     = "ptsvs";
  $dbpassword = "svspt";
  $database = "ptlink_services";
                                
  /* end of configuration, don't touch below */                                
  require_once("$appdir/includes/db.php");
  require_once("$appdir/includes/lang.php");
?>
