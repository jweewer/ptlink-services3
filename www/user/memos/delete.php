<?php
require_once("../../includes/config.inc.php"); 
require_once("../../includes/session.inc.php");

$link_id = db_connect();
$query = "DELETE FROM memoserv WHERE owner_snid = ".$_SESSION["user"]->snid ;
$query .= " AND flags = '0'";
$query .= " AND id =".(int)$_REQUEST["id"];
$result = mysql_query($query, $link_id) or die("Unable to update memos - Database troubles");
header("Location: list.php");
exit;
?>