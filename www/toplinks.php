<?php require_once("includes/config.inc.php"); ?>
<?php require_once("includes/ns_flags.inc.php"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/
<html>
<head>
<title>PTlink IRC Services - <? echo L_TOP_LINKS; ?></title>
</head>
<head>
<link rel="stylesheet" href="stylesheet/styles.css" type="text/css" media="screen, projection">
</head>
<body>
<?php include_once ("top_main.php"); ?>
<table align="center" class="width50" cellspacing="1">  
<tr>
  <td align="center" class="field" colspan="2"><?php echo L_TOP_LINKS; ?></td>
</tr>
<tr class="row-category">
<?php 
  echo "<td>".L_LINK."</td><td>".L_RECOMMENDED."</td></tr>";
  $link_id = db_connect();

  $query = " SELECT favlink, count(0) as c FROM nickserv WHERE favlink IS NOT NULL";
  $query .= " AND NOT (flags & ".$NFL_FORBIDDEN.") AND (flags & ".$NFL_AUTHENTIC.")";
  $query .= " GROUP BY favlink ORDER BY c DESC LIMIT 100";
  $result =mysql_query($query, $link_id) 
    or die("Unable list links- Database troubles");
  $rc = mysql_num_rows($result);
  $i = 0;
  while ($row = mysql_fetch_object($result))
    {
      echo "<tr class=\"row-".($i % 2)."\">";
      $link = $row->favlink;
      if(strncasecmp($link, "http://", 7))
        $link = "http://".$link;
      echo "<td align=\"center\"><A HREF=\"".$link."\">";
      echo htmlentities($row->favlink)."</A></td>";
      echo "<td align=\"center\">".$row->c."</td>";
      echo "</tr>\n";
      $i++;
    }
  mysql_free_result($result);  
?>
</table>
<br>
<?php include_once("includes/footer.php"); ?>
</body>
</html>
