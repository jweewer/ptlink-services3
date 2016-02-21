<?php 
  require_once("../includes/config.inc.php");
  require_once("../includes/session.inc.php"); 
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/
<html>
<head>
<title>PTlink IRC Services</title>
</head>
<head>
<link rel="stylesheet" href="../stylesheet/styles.css" type="text/css" media="screen, projection">
</head>
<body>
<?php
  include_once ("top.php");
  $link_id = db_connect();
  $query = "SELECT count(*) FROM memoserv WHERE owner_snid=$snid ";
  $query .= " AND (flags & 1)";
  $result = mysql_query($query, $link_id) or die("Unable list memos");
  $row = mysql_fetch_row($result);
  mysql_free_result($result);
  $count = $row[0];
  if($count > 0)
  {
    echo "<p align=\"center\">";
    echo L_YOU_HAVE." <b>".$count."</b> <A HREF=\"memos/list.php\">".
      L_NEW_MEMOS."</A>.";
    echo "</p>";
  }
  include_once("../includes/footer.php"); 
?>
</html>
</body>
