<?php
require_once("../../includes/config.inc.php"); 
require_once("../../includes/session.inc.php");

$link_id = db_connect();
$read = (int)~1;
$query = "UPDATE memoserv SET flags = flags & ".$read." WHERE owner_snid = ".$_SESSION["user"]->snid ;
$query .= " AND flags & 1";
if($_GET["id"] != "")
  $query .= " AND id=". (int)$_GET["id"];
$result = mysql_query($query, $link_id) or die("Unable to update memos - Database troubles");

header("Location: list.php");
exit;
?>