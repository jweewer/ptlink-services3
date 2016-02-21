<?php
// sqlbuild functions
//
//
$sqlb_table = "";


/* init the structures and set the table name */
function sqlb_init($table)
{
  global $sqlb_table, $sqlb_fcount, $sqlb_data, $sqlb_types;
  $sqlb_fcount = 0;
  $sqlb_data = array();
  $sqlb_types = array();  
  $sqlb_table = $table;
  $sqlb_fcount = 0;
}

function sqlb_add_str($field, $value)
{
  global $sqlb_fcount, $sqlb_data, $sqlb_types;
  $sqlb_data[$field] = $value;
  $sqlb_types[$sqlb_fcount] = 1;
  $sqlb_fcount++;  
}

function sqlb_add_int($field, $value)
{
  global $sqlb_fcount, $sqlb_data, $sqlb_types;
  $sqlb_data[$field] = $value;
  $sqlb_types[$sqlb_fcount] = 2;
  $sqlb_fcount++;  
}

function sqlb_insert()
{
  global $sqlb_table, $sqlb_fcount, $sqlb_data, $sqlb_types;
  $insert = "INSERT INTO ".$sqlb_table. "(";
  $i = 0;
  $fields = "";
  $values = "";
  foreach ($sqlb_data as $key=>$value)
  {
    if(!empty($fields))
       $fields .= ",";
    $fields .= $key;
    if(!empty($values))
       $values .= ",";    
    if($sqlb_types[$i] == 1)
      $values .= "'".mysql_real_escape_string($value)."'";
    else 
      $values .=  intval($value);
    $i++;
  }
  return $insert.$fields.") VALUES (".$values.")";
}

?>
