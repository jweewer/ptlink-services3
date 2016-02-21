<?php
require_once("$appdir/classes/user.class.php");
session_start();

if(!isset($_SESSION["user"])) {
  header("Location: user_logout.php");
//	die("Not logged in");
}
if(!is_numeric($_SESSION["user"]->snid)) {
  header("Location: $appdir/user_logout.php");
//echo"<p>Click <A HREF="index.php">here</A> to login.</p>
//	die("Not logged in");
}
if($_SESSION["IP"] != $_SERVER["REMOTE_ADDR"]) {
	header("Location: $appdir/user_logout.php");
}
if((time() - $_SESSION["timestamp"]) > 1800) {
	header("Location: $appdir/user_logout.php?timeOut=1");
}
else {
	$_SESSION["timestamp"] = time();
	$snid = $_SESSION["user"]->snid;
}
?>
