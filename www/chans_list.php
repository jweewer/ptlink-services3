<?php require_once("includes/config.inc.php"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/
<html>
<head>
<title>PTlink IRC Services - <? echo L_LIST_CHANS; ?></title>
</head>
<head>
<link rel="stylesheet" href="stylesheet/styles.css" type="text/css" media="screen, projection">
</head>
<body>
<?php include_once ("top_main.php"); ?>
<table class="width100" cellspacing="1">  
<tr>
  <td align="center" class="field" colspan="2"><?php echo L_LIST_CHANS; ?></td>
</tr>
<tr class="row-category">
<?php 
  echo "<td>".L_CHAN."</td><td>".L_LAST_TOPIC."</td></tr>";
  $link_id = db_connect();

  $query = "SELECT name, last_topic FROM chanserv ";
  $query .= "WHERE NOT (flags & 1) ORDER BY name";
  $result =mysql_query($query, $link_id) 
    or die("Unable list chans- Database troubles");
  $rc = mysql_num_rows($result);
  $i = 0;
  while ($row = mysql_fetch_object($result))
    {
      echo "<tr class=\"row-".($i % 2)."\">";
      echo "<td>".htmlentities($row->name)."</td>";
      echo "<td>".htmlentities($row->last_topic)."</td>";
      echo "</tr>\n";
      $i++;
    }
  mysql_free_result($result);
?>
</table>
<br>
<? include_once("includes/footer.php"); ?>
</body>
</html>
