<?php
  require("includes/config.inc.php");  
  $user = $_GET['user'];
  $link_id = db_connect();
  $query = "SELECT p.photo,p.status FROM nickserv n,ns_photo p";
  $query .= " WHERE p.snid=n.snid AND n.nick='".mysql_escape_string(strtolower($user))."'";
  $result = mysql_query($query, $link_id) or die("0");    
  if(mysql_num_rows($result)<1)
    die("0");
  $row = mysql_fetch_array($result);
  mysql_free_result($result);
  if($row[1] != "P")
    die("0");
  $jpg = $row[0];
  header("Content-type: image/jpeg"); // act as a jpg file to browser      
  echo $jpg;
  mysql_close($link_id);
?>
