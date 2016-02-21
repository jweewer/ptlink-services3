<?php
  //error_reporting(E_ALL | E_STRICT);
  function Error($message)
  {
    $_SESSION["regerr"] = $message;
    header("Location: index.php");
    die;
  }
  session_start();
  unset($_SESSION["regerr"]);
  require_once("../includes/config.inc.php");
  require_once("../includes/utils.inc.php");
  $email = $_POST["email"];
  $user = $_POST["user"];
  $pass = $_POST["password"];
  $regcode = $_POST["regcode"];  
  $req_regcode = $_SESSION["regcode"];
  unset($_SESSION["regcode"]);
  if(!ValidUser($user))
    Error(L_INVALID_USER);
  else
  if(!ValidEmail($email))
    Error(L_INVALID_EMAIL);
  else
  if(strlen($pass) < 2)
    Error(L_INVALID_PASS);
  else
  if((strlen($regcode)<2) || ($regcode != $req_regcode))
    Error(L_INVALID_REGCODE);
  else
  {
    $link_id = db_connect();
    $loginQuery = "SELECT nick FROM nickserv WHERE nick='";
    $loginQuery .=  strtolower(addslashes($user))."'";  
    $loginResult = mysql_query($loginQuery, $link_id) 
      or die("Unable to check user - Database troubles</body></html");
    if(mysql_num_rows($loginResult) > 0)
    {
      Error(L_USER_ALREADY_EXISTS);
      die();
    } 
    mysql_free_result($loginResult);
    $loginQuery = "SELECT email FROM nickserv WHERE email='";
    $loginQuery .=  strtolower(addslashes($email))."'";  
    $loginResult = mysql_query($loginQuery, $link_id) 
      or die("Unable to check the email - Database troubles</body></html");
    if(mysql_num_rows($loginResult) > 0)
    {
      Error(L_EMAIL_ALREADY_EXISTS);
    }
    mysql_free_result($loginResult);
  }
  $expire_time = time_str(dbconf_get("nickserv", "ExpireTime"));
  $sec_len = dbconf_get("nickserv", "SecurityCodeLenght");
  $auth= rand_string($sec_len, $sec_len);
  //die("Expire time is $expire_time");
  include_once("../includes/sqlb.php");
  /* validation completed, lets insert */
  // Insert user
  sqlb_init("nickserv");
  sqlb_add_str("nick", strtolower($user));
  sqlb_add_str("email", $email);
  sqlb_add_int("flags", 0);
  sqlb_add_int("status", 0);
  sqlb_add_int("t_reg", time());
  sqlb_add_int("t_ident", time());
  sqlb_add_int("t_seen", time());
  sqlb_add_int("t_expire", time()+$expire_time);
  sqlb_add_int("lang", 0);  
  if(!mysql_query(sqlb_insert()))
   die("Error 1: inserting user, please contat the system administrator");
  $snid = mysql_insert_id();   
  // Record login
  sqlb_init("ns_last");
  sqlb_add_int("snid", $snid);
  sqlb_add_str("web", "y");
  sqlb_add_int("t_when", time());
  sqlb_add_str("realhost", $_SERVER["REMOTE_ADDR"]);
  mysql_query(sqlb_insert());   
  // Add nick security info
  sqlb_init("nickserv_security");
  sqlb_add_int("snid", $snid);
  sqlb_add_str("pass", md5($pass));
  sqlb_add_str("securitycode", md5($auth));
  sqlb_add_int("t_lset_pass", time());
  sqlb_add_int("t_lauth", time());
  if(!mysql_query(sqlb_insert()))
   die("Error 2: inserting user, please contat the system administrator");          
  $email_from = dbconf_get("email", "EmailFrom");
  $email_from_name = dbconf_get("email", "EmailFromName");
  $subject = L_REGISTER_SUBJECT." ".$user;
  $headers = "From: \"".$email_from_name."\" <".$email_from.">";
  $message = L_REGISTER_EMAIL_MSG1." $auth .\n";
  $message .= L_REGISTER_EMAIL_MSG2;
  $message .= L_REGISTER_EMAIL_MSG3;
  $message .= L_REGISTER_EMAIL_MSG4;
  $message .= "$appurl/register/auth.php?id=".$snid."&c=".md5($auth);
  mail($email, $subject, $message, $headers);
  echo "<br><br><br>";
  echo "<p align=\"center\">".L_REGISTER_EMAIL_SENT_TO." <B>$email</B> ".L_REGISTER_EMAIL_CHECK;
  echo "<br>".L_REGISTER_PROCEED." <A HREF=\"../index.php\">".L_HERE."</A><br></p>";  
  // record to the stats
  mysql_query("UPDATE recordstats SET ns_new_web=cs_new_web+1, cs_total=cs_total+1 WHERE day=CURDATE()");
?>
