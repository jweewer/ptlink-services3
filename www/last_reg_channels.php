<?php require_once("includes/config.inc.php"); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/
<html>
<head>
<title>PTlink IRC Services - Memos list</title>
</head>
<head>
<link rel="stylesheet" href="stylesheet/styles.css" type="text/css" media="screen, projection">
</head>
<body>
<?php 
  include_once ("top_main.php");
?>
<br>
<table class="width100" cellspacing="1">  
<tr>
  <td align="center" class="field" colspan="4"><?php echo L_LAST_REG_CHANS; ?></td>
</tr>
<tr class="row-category">
<?php
$link_id = db_connect();
$query = "SELECT name, last_topic, cdesc, FROM_UNIXTIME(t_reg) t_s FROM chanserv ORDER BY t_reg DESC LIMIT 0,10";
$result = mysql_query($query, $link_id) or die("Unable to read channels - Database troubles");
$rc = mysql_num_rows($result);
echo "<td class=\"row-id\" width=\"150\">".L_CHAN_NAME."</td><td width=\"180\">".L_DATE."</td><td width=\"200\">".L_CHAN_DESC."</td><td >".L_CHAN_TOPIC."</td>";
$i = 0;
	while ($row = mysql_fetch_object($result)){
		echo "<tr class=\"row-".($i % 2)."\">";
		echo "<td>$row->name</td>";
		echo "<td>$row->t_s</td>";
		echo "<td>";
		if ($row->cdesc == '') {
			echo "<i>". L_NO_CHAN_DESC ."</i>";
		}
		else {
			$cdesc = htmlentities($row->cdesc);
			echo "<td>$htmlentities</td>";
		}
		echo "<td>";
		if ($row->last_topic == '') {
			echo "<i>". L_NO_TOPIC_SET ."</i>";
		}
		else {
			$topic = htmlentities($row->last_topic);
			echo $topic;
		}
		echo "</td>";
	}

echo "</table><br>";

include_once("includes/footer.php");
?>
</body>
</html>

