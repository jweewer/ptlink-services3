<?php
  require("../includes/config.inc.php");
  require("../includes/session.inc.php");
  $value=$_GET['c'];
  if($value=="y")
  {
    $link_id = db_connect();
    $query = "DELETE FROM ns_photo WHERE snid=$snid AND status<>'B'";
    $result = mysql_query($query, $link_id);
    echo "L_PHOTO_DELETED";
    echo "<script language=\"JavaScript\">parent.location = \"index.php\"</script>";
  }
  else 
  {
    echo L_WANT_TO_DELETE." ("."<A HREF=\"delete_photo.php?c=y\">".L_YES."</A>,";
    echo "<A HREF=\"index.php\">".L_NO."</A>)";
   }
?>
