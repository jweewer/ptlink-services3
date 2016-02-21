<?php
function ValidEmail($Email)
{
  return eregi("^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$", $Email);
}

function ValidUser($User)
{
  if(strlen($User)>16)
    return false;
  return eregi("^[a-z]+([a-z0-9])*$", $User);
}

function time_str($str)
{
  $i = intval($str);
  if(strchr($str, 'm'))
    $i = $i*60;    
  else
  if(strchr($str, 'h'))
    $i = $i*3600;  
  else
  if(strchr($str, 'd'))
    $i = $i*24*3600;
  else
  if(strchr($str, 'M'))
    $i = $i*30*24*3600;    
  else
  if(strchr($str, 'Y'))
    $i = $i*365*30*24*3600;
    
  return $i;
}

function rand_string($minlen, $maxlen)
{
  $len = rand($minlen, $maxlen);
  $rstring = "";
  for($i=0; $i<$len; ++$i)
    $rstring .= chr(ord('a')+rand(0, ord('z')-ord('a')));
  
  return $rstring;
}
?>
