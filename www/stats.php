<? require_once("includes/config.inc.php"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <title><?php echo L_STATS; ?></title>
    <link rel="stylesheet" href="stylesheet/styles.css" type="text/css" media="screen, projection">
  </head>                                                
<body>
<p align="center" class="header"><?php echo L_STATS; ?></p>
<?php
  include("top_main.php");
?>
  <p align="center">
  <IMG SRC="stats/nicks_total_last_month.php">
  </p>
<?php
  include_once("includes/footer.php");
?>
</body>
</html>
