<table align="center"><tr>
<?php 
  if(file_exists("../includes/config.inc.php"))
    require_once("../includes/config.inc.php");
  else
    require_once("../../includes/config.inc.php");
  $link_id = db_connect();
  $query = "SELECT id, status FROM ns_photo WHERE snid=$snid";
  $result = mysql_query($query, $link_id);
  $row = mysql_fetch_object($result);
  $rowc = mysql_num_rows($result);
  if($rowc > 0)
    {
      echo "<td>";
      echo "<A HREF=\"$appurl/view_photo.php?id=".$row->id."&s=1\">";
      echo "<IMG SRC=\"$appurl/view_photo.php?id=".$row->id."&t=1\"></A></td>";
    }
  mysql_free_result($result);
  mysql_close($link_id);
  echo "<td>";
  echo L_WELCOME." ".$_SESSION["user"]->nick;
  echo "</td>";
?>
</tr></table>
<br>
<table class="width100" cellspacing="0">
<tr><td class="menu">
<?php
echo "<A HREF=\"$appurl/user/index.php\">".L_MAIN."</A> | ";
echo "<A HREF=\"$appurl/user/memos/list.php\">".L_LIST_MEMOS."</A> | ";
  if($rowc==0)
    $upd = 0;
  else
    $upd = 1;
  echo "<A HREF=\"$appurl/user/upload_photo.php?upd=$upd\">".L_UPLOAD_PHOTO."</A>";
  echo " | ";
  if($upd == 1)
    {
      echo "<A HREF=\"$appurl/user/delete_photo.php\">".L_DELETE_PHOTO."</A>";
      echo " | ";
    }
echo "<A HREF=\"$appurl/user_logout.php\">".L_LOGOUT."</A>";
?>
</td></tr>
</table>
<br>
