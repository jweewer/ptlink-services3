<?php
  if(!file_exists("../includes/config.inc.php"))
    die("No configuration found at includes/config.inc.php");
  require_once("../includes/config.inc.php");
  require_once("../includes/ns_flags.inc.php");
?>        
<?php
// auth.php
// this script allows a nick to authenticat its email address
// parmeters are read from GET
// id = snid of the nick
// c = md5sum of the security code wich was sent to the user
  $snid = intval($_GET['id']);
  $c = $_GET['c'];
  if(!$snid || !$c)
    die("Missing parameters");
  $link_id = db_connect();    
  $c = mysql_real_escape_string($c);
  $result = mysql_query("SELECT count(*) FROM nickserv_security WHERE snid=$snid AND securitycode='$c'")
      or die("Error connecting to the db");
  if(!$result)
    die("Error connecting to the db (2)");
  $row = mysql_fetch_array($result);
  mysql_free_result($result);
  if(intval($row[0]) == 1)
  {
    mysql_query("UPDATE nickserv SET flags=(flags | $NFL_AUTHENTIC) WHERE snid=$snid")
      or die("Error updating the db");
    echo "<p align=\"center\">".L_EMAIL_VERIFIED." <A HREF=\"../index.php\">".L_HERE."</A> .</P>";
    //echo "Your email address was verified, you can login");
  }
  else
    die("Invalid/expired information!");
?>

