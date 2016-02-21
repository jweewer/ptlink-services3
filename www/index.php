<?php
  if(!file_exists("includes/config.inc.php"))
    die("No configuration found at includes/config.inc.php");
  require_once("includes/config.inc.php");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>
  <head>
    <title>[<?php echo $main_title ?>] - login</title>
    <link rel="stylesheet" href="stylesheet/styles.css" type="text/css">
  </head>
<body>
<?php include("top_main.php"); ?>
  <p align="center" class="header"><?php echo $main_title; ?></p>
  <form action="user_login_action.php" id="loginForm" method="post">  
  <table align="center">  
    <tr>
      <td class="field"><?php echo L_USER; ?></td>
    </tr>
    <tr>
        <td><input type="text" name="nick"></td>      
    </tr>
    <tr><td height="20"></td></tr>
    <tr>
      <td class="field"><?php echo L_PASS; ?></td>
    </tr>
    <tr>
	<td><input name="password" type="password"></td>
    </tr>
    <tr>
      <td align="center" height="49"><input name="userLogin" value="<?php echo L_LOGIN; ?>" type="submit"></td>
    </tr>    
  </table>
  <input type="hidden" name="lang" value="<?php echo $lang ; ?>">
</form>
<?php 
  echo "<p align=\"center\">".L_TO_REGISTER."</p>";
  $link_id = db_connect();
  $query = "SELECT count(*) FROM nickserv WHERE status & 1 <>0";
  $result = mysql_query($query, $link_id) or die("Error connecting to the db");
  $row = mysql_fetch_array($result);  
  $online_count = $row[0];
  $query = "SELECT count(*) FROM nickserv";
  $result = mysql_query($query, $link_id) or die("Error connecting to the db");
  $row = mysql_fetch_array($result);
  $total_count = $row[0];
  echo "<p align=\"center\"><b>$total_count</b> ".L_REG_USERS."<br>";
  echo "<b>$online_count</b> ".L_ARE_ONLINE."</p>";
  mysql_free_result($result);
  mysql_close($link_id);
?>

<?php if($lang != "en_us")
  echo "<p align=\"center\" class=\"italic-small\">Press below to change to english";
  echo "</p>";
?>
<table align="center">
<tr>
<td>
<form method="post" name="langform" action="index.php">
<input type="submit" class="button-small" value="<?php echo L_CHANGE_LANG; ?>">
<select name="lang" class="small">
	<option value="en_us">
		<?php echo L_ENGLISH ?>
	</option>
	<option value="nl">
		<?php echo L_DUTCH ?>
	</option>
	<option value="pt">
		<?php echo L_PORTUGUESE ?>
	</option>
</select>
</form>
</td></tr>
</table>
<?php include_once("includes/footer.php"); ?>
</body>
</html>
