<?php
  require("includes/config.inc.php");  
  $id = $_GET['id'];
  $t =  $_GET['t'];
  $s = $_GET['s'];
  $link_id = db_connect();
  $query = "SELECT ";
  if($t == 1)
    $query .= "thumb";
  else
    $query .= "photo";
  $query .= " , status FROM ns_photo WHERE id=".mysql_escape_string($id);
  //echo "$query";
  $result = mysql_query($query, $link_id) or die("Photo was not found!");    
  if(mysql_num_rows($result)<1)
    die("Photo was not found!");
  $row = mysql_fetch_array($result);
  mysql_free_result($result);
  if($row[1] != "P")
    die("Photo was not found!");
  $jpg = $row[0];
  if($t != 1 && $s!= 1) // is not a thumb and is not silent click
    {
      $query = "UPDATE ns_photo SET hits=hits+1 WHERE id=".mysql_escape_string($id);
      mysql_query($query, $link_id);
    }
  header("Content-type: image/jpeg"); // act as a jpg file to browser      
  echo $jpg;
  mysql_close($link_id);
?>
