<?php 
  require_once("../../includes/config.inc.php");
  require_once("../../includes/session.inc.php"); 
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/
<html>
<head>
<title>PTlink IRC Services - Memos list</title>
</head>
<head>
<link rel="stylesheet" href="../../stylesheet/styles.css" type="text/css" media="screen, projection">
</head>
<body>
<?php 
  include_once ("../top.php"); 
  $link_id = db_connect();
  $query = "SELECT count(*) FROM memoserv WHERE owner_snid=$snid ";
  $query .= " AND (flags & 1)";
  $result = mysql_query($query, $link_id) or die("Unable list memos");
  $row = mysql_fetch_row($result);
  $count = $row[0];
  if($count > 0)
  {
    echo "<p align=\"center\">";
    echo L_YOU_HAVE." <b>".$count."</b> ".L_NEW_MEMOS.".";
    echo "</p>";
  }
  mysql_free_result($result);
?>
<table class="width100" cellspacing="1">  
<tr>
  <td align="center" class="field" colspan="6"><?php echo L_YOUR_MEMOS; ?></td>
</tr>
<tr class="row-category">
<?php 
  $query = "SELECT id, sender_name, flags, FROM_UNIXTIME(t_send) t_s, message FROM memoserv ";
  $query .= "WHERE owner_snid=".$_SESSION["user"]->snid." ORDER by (flags & 1) DESC, t_s ASC";    
  $result =mysql_query($query, $link_id) or die("Unable list memos- Database troubles");
  $rc = mysql_num_rows($result);
  $i = 0;
  $unread = false;
  echo "<td width=\"20\">".L_ID."</td><td width=\"20\"></td><td width=\"150\">".L_DATE."</td><td width=\"120\">".L_FROM."</td><td>".L_MESSAGE."</td>";
  echo "<td width=\"20\">&nbsp;</td></tr>";  
  while ($row = mysql_fetch_object($result))
    {
      echo "<tr class=\"row-".($i % 2)."\">";
      echo "<td>$row->id</td>";
      echo "<td>";
      if($row->flags & 1)
      {
        echo "<A HREF=\"mark_read.php?id=".$row->id."\">";
        echo "<IMG SRC=\"$appurl/ic/closed_envelop.png\">";
        echo "</A>";
      }
      else
        echo "<IMG SRC=\"$appurl/ic/open_envelop.png\">";
      echo "</td>";
      echo "<td align=\"center\">$row->t_s</td>";
      echo "<td akign=\"center\">";
      echo "$row->sender_name</td>";
      $message = htmlentities($row->message);
      echo "<td>$message</td>";
      echo "<td align=\"left\">";
      echo "<a href=\"delete.php?id=". $row->id ."\"><img SRC=\"$appurl/ic/trash.png\" border=\"0\" alt=\"". L_DEL_MEMO ."\"></a>";
      echo "</td>";
      if($row->flags & 1) 
        $unread = true;
      echo "</tr>\n";
      $i++;
    }
  mysql_free_result($result);
?>

</table>
<?php
  if($unread)
    echo "<A HREF=\"mark_read.php\">".L_MARK_READ."</A><br>";
?>
<br>
<table class="width100" cellspacing="1">  
<tr>
  <td align="center" class="field" colspan="5"><?php echo L_SENT_MEMOS; ?></td>
</tr>
<tr class="row-category">
<?php 
  $query = "SELECT ms.id, ns.nick, ms.flags, ms.owner_snid, FROM_UNIXTIME(ms.t_send) t_s, ms.message  FROM memoserv ms, nickserv ns";
  $query .= " WHERE ms.sender_snid=".$_SESSION["user"]->snid;
  $query .= " AND ns.snid=ms.owner_snid AND (ms.flags & 1) ORDER by t_s ASC";
  $result = mysql_query($query, $link_id) or die("Unable list memos- Database troubles");
  $rc = mysql_num_rows($result);
  $i = 0;
  echo "<td width=\"20\">".L_ID."</td><td width=\"150\">".L_DATE."</td><td width=\"120\">".L_TO."</td><td>".L_MESSAGE."</td>";
  echo "<td width=\"20\">&nbsp;</td></tr>";
  while ($row = mysql_fetch_object($result))
    {
      echo "<tr class=\"row-".($i % 2)."\">";
      echo "<td>$row->id</td>";
      echo "<td align=\"center\">$row->t_s</td>";
      echo "<td>$row->nick</td>";
      $message = htmlentities($row->message);
      echo "<td>$message</td>";
      echo "<td><a href=\"cancel.php?id=". $row->id ."&owner=". $row->owner_snid ."\"><img SRC=\"$appurl/ic/trash.png\" border=\"0\" alt=\"". L_DEL_MEMO ."\"></a></td>";
      echo "</tr>\n";
      $i++;    
    }
  mysql_free_result($result);  
  mysql_close($link_id);  
  echo "</table><br>";
  include_once("../../includes/footer.php");

?>

</body>
</html>
