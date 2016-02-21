<?php 
  require_once("../includes/config.inc.php");
  require_once("../includes/session.inc.php");  
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/
<html>
<head>
<title>PTlink IRC Services - <?php echo L_UPLOAD_PHOTO; ?></title>
</head>
<head>
<link rel="stylesheet" href="../stylesheet/styles.css" type="text/css" media="screen, projection">
</head>
<body>
<?php
  include_once ("top.php");
  $upd = $_GET['upd'];
  if($upd==0)
    echo L_ADDING_PHOTO."<br>";
  else
    echo L_UPDATING_PHOTO."<br>";
?>
<HTML>
<form enctype="multipart/form-data" method="post" action="insert_photo.php" name="form1">
<input type="hidden" name="MAX_FILE_SIZE" value="102400">
<input type="file" name="photo" accept="image/jpeg">
<input type="hidden" name="upd" value="<?php echo $upd ?>">
<input type="submit" value="<?php echo L_UPLOAD; ?>">
</form>
<?php include_once("../includes/footer.php"); ?>
</HTML>
