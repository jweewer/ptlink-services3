<?php
if(isset($_GET["lang"]))
  $lang = $_GET["lang"];
else if(isset($_POST["lang"])) {
	$lang = $_POST["lang"];
	//echo "Trying to force cookie to $lang";
	setcookie("lang", $lang, time()+30*24*3600, '/');	
	//setcookie ("lang");	
	}
else if(isset($_COOKIE["lang"])) {
	$lang = $_COOKIE["lang"];
	}
else {
	$lang = $default_language;
	}

switch ($lang) {
	case "en_us":
		include("$appdir/langs/en_us.php");
		break;
	case "nl":
		include("$appdir/langs/nl.php");
		break;
	case "pt":
		include("$appdir/langs/pt.php");
		break;
	default:
		include("$appdir/langs/en_us.php");
		break;
}
?>
