<?php
/* db auxiliary functions
*/
  function db_connect() { 
        global $dbhost, $dbuser, $dbpassword, $database;
        $link_id = mysql_connect($dbhost, $dbuser, $dbpassword)
          or die("FATAL ERROR: Could not contact the database server!");
        @mysql_select_db($database)
          or die("FATAL ERROR: There was a problem with the database!");
        return $link_id;
  }
  
function dbconf_get($module, $item)
{
  /* lets escape just to be safe */
  $module = mysql_real_escape_string($module);
  $item  = mysql_real_escape_string($item);
  $result = mysql_query("SELECT value FROM dbconf WHERE module='".$module.
    "' AND name='".$item."'") or
    die("Error on query to get dbconf item ".$module.".".$item);
  if(!$result)
    die("Unable to get dbconf item ".$module.".".$item);
  $row = mysql_fetch_row($result);
  mysql_free_result($result);
  return $row[0];
}

?>
