<?php
session_start();

/** destroy session data */
$_SESSION = array();
/** destroy session */
session_destroy();
/** destroy cookie */
unset($_COOKIE[session_name()]);
header("Location: index.php");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>
	<head>
		<title>[protected site] - logged out</title>
		<link rel="stylesheet" href="stylesheet/styles.css" type="text/css" media="screen, projection" />
	</head>
	<body>
		<h1><?php echo isset($_GET["timeOut"]) ? "Timed Out" : "Logged Out"; ?></h1>
		<p>You are now logged out.</p>
		<p>Click <A HREF="index.php">here</A> to login.</p>
		
	</body>
</html>
<script language="JavaScript">
   if (self.parent.frames.length != 0)
   	self.parent.location="index.php";
</script>
<?php
  header("Location: index.php");
?>
    
