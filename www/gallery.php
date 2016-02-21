<? require_once("includes/config.inc.php"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <title><?php echo L_COM_PHOTOS; ?></title>
    <link rel="stylesheet" href="stylesheet/styles.css" type="text/css" media="screen, projection">
  </head>                                                
<body>
<p align="center" class="header"><?php echo L_COM_PHOTOS; ?></p>
<?php
  include("top_main.php");
  $link_id = db_connect();
  $query = "SELECT p.id, p.thumb, p.t_update, p.hits, ns.nick FROM ns_photo p, nickserv ns";
  $query .= " WHERE p.status='P' AND ns.snid=p.snid ORDER BY p.t_update DESC";
  $result = mysql_query($query, $link_id) or die("Photo was not found!");    
//  if(mysql_num_rows($result)<1)
//    die("No photos were found!");
  echo "<table align=\"center\" border=1><tr><td>";    
  echo "<table>";
  $count = 0;
  while ($row = mysql_fetch_object($result))
    {
      if($count % 8 == 0) 
        {
          if($count > 0)
            echo "</tr>\n";
          echo "<tr>\n"; 
        }
      echo "<td><center>";
      echo "<A HREF=\"view_photo.php?id=".$row->id."\">";
      echo "<IMG SRC=\"view_photo.php?id=".$row->id."&t=1\">";
      echo "<br>";
      echo $row->nick."</A><br>";      
      echo "<font size=\"-1\">";
      echo date("d/m/Y", strtotime($row->t_update));
      echo "<br>";
      echo "<b>".$row->hits."</b> hit(s)";
      echo "</font>";      
      echo "</center><td>\n";      
      $count++;
    }  
  echo "</tr></table>";
  echo "</td></tr></table><br>";  
  echo "<p align=\"center\">".L_GALLERY_BOTTOM_MSG."</p><br>";
  mysql_free_result($result);  
  mysql_close($link_id);
  include_once("includes/footer.php");
?>
</body>
</html>
        
