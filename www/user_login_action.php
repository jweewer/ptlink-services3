<?php
require("includes/config.inc.php");
require("includes/utils.inc.php");
require("includes/sqlb.php");
if(isset($_POST["userLogin"])) {
	if(strlen(trim($_POST["nick"])) > 0) {
		if(strlen(trim($_POST["password"])) > 0) {
			$link_id = db_connect();
			$loginQuery = "SELECT n.snid, nick, email FROM nickserv n, nickserv_security ns";
			$loginQuery .= " WHERE ns.snid=n.snid AND";
			$loginQuery .= " LCASE(nick) = '". strtolower(addslashes($_POST["nick"])) . "' AND ";
			$loginQuery .= " pass = '". md5($_POST["password"]) . "'";
			//$loginQuery .= " AND (flags & 0x00000020)<> 0";  // must be authenticated
			$loginResult = mysql_query($loginQuery, $link_id) or die("Unable to check your id - Database troubles</body></html");
			if(mysql_num_rows($loginResult) == 1) {
				require("classes/user.class.php");
				$userData = mysql_fetch_object($loginResult);
				mysql_free_result( $loginResult);
				sqlb_init("ns_last");
				sqlb_add_int("snid", $userData->snid);
				sqlb_add_str("web", "y");
				sqlb_add_int("t_when", time());
				sqlb_add_str("realhost", $_SERVER["REMOTE_ADDR"]);
				mysql_query(sqlb_insert());
				$seen = time();
				$expire_time = time_str(dbconf_get("nickserv", "ExpireTime"));
				if($expire_time > 0)
				  $expire =  $seen + $expire_time;
				else
				  $expire = 0;
				$sql = "UPDATE nickserv SET t_seen=$seen, t_expire=$expire";
				$sql .= " WHERE snid=".$userData->snid;
				mysql_query($sql);
				session_start();
				session_regenerate_id();
				$_SESSION["user"] = new User($userData->snid, $_POST["nick"], $userData->email);
				$_SESSION["IP"] = $_SERVER["REMOTE_ADDR"];
				$_SESSION["timestamp"] = time();
				header("Location: user/index.php");
				//echo "<h1>Logged In</h1>";
				//echo "Welcome, " . $_SESSION["user"]->nick . "! Your login will proceed in 5 s.<a href=\"user_main.php\">protected document</a>.";
				
			}
			else {
				mysql_close($link_id);
				echo L_INC_NICK_PASS;
			}
		}
		else {
			echo L_EMPTY_PASS;
		}
	}
	else {
		echo L_EMPTY_NICK;
	}
}
else {
	echo L_INV_FORM;
}
?>
<p><?php echo L_CAN_LOGIN; ?> <A HREF="index.php"><?php echo L_HERE; ?></A> .</p>

