<?php
  session_start();
  $_SESSION["regcode"] = rand(100000,999999);
  require_once("../includes/config.inc.php");    
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>
  <head>
    <title>[<?php echo $main_title ?>] - Register</title>
    <link rel="stylesheet" href="../stylesheet/styles.css" type="text/css">
  </head>
<body>
  <p align="center" class="header"><?php echo L_NEW_USER; ?></p>
<?php 
  if($_SESSION["regerr"])
  {
    echo "<font color=\"#FF3300\">";
    echo "<p align=\"center\">".$_SESSION["regerr"]." !</p>";
    echo "</font>";
    unset($_SESSION["regerr"]);
  }
?>
  <form action="register.php" id="loginForm" method="post">  
  <table align="center">  
    <tr>
      <td class="field"><?php echo L_USER; ?></td>
    </tr>
    <tr>
        <td><input type="text" name="user" size=16></td>      
    </tr>
    <tr>
      <td class="field"><?php echo L_EMAIL; ?></td>
    </tr>
    <tr>
        <td><input type="text" name="email"></td>      
    </tr>
    <tr>
      <td class="field"><?php echo L_PASS; ?></td>
    </tr>
    <tr>
	<td><input name="password" type="password" size=16></td>
    </tr>
    <tr><td height=20></td></tr>
  </table>
  <p align="center">
  <?php echo L_TYPE_CODE; ?><br>
  <IMG SRC="regcode.php"><br>
  <input type="text" name="regcode" size="6"><br><br>
  <input name="register" value="  <?php echo L_REGISTER; ?>  " type="submit"><br>
  <input type="hidden" name="lang" value="<?php echo $lang ; ?>">
</form>
<?php include_once("../includes/footer.php"); ?>
</body>
</html>
