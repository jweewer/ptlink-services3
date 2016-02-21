<?php
require_once("../../includes/config.inc.php"); 
require_once("../../includes/session.inc.php");

$link_id = db_connect();
$query = "DELETE FROM memoserv WHERE sender_snid = ".$_SESSION["user"]->snid ;
$query .= " AND flags = '1'";
$query .= " AND id =".(int)$_REQUEST["id"];
$query .= " AND owner_snid =".(int)$_REQUEST["owner"];
$result = mysql_query($query, $link_id) or die("Unable to delete memo - Database troubles");
header("Location: list.php");
exit;
?>