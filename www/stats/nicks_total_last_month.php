<? require_once("../includes/config.inc.php"); ?>
<? require_once("../includes/panachart.php"); ?>
<?php
  $link_id = db_connect();
  $query = "SELECT DATE_FORMAT(day, '%d/%m') as day, ns_total, ns_new_irc, ns_lost FROM recordstats";
  //$query .= " ORDER BY DATE_FORMAT(day, '%y%m%d') LIMIT 30";
  $query .= " ORDER BY DATE_FORMAT(day, '%y%m%d') DESC LIMIT 30";
  $result = mysql_query($query, $link_id) or die("Error reading from db!");    
  if(mysql_num_rows($result)<1)
    die("No data");
  //echo "<center><table border=1><tr><td>";    
  //echo "<table>";
  $max = 0;
  $xlabel = array();
  $data = array();  
  while ($row = mysql_fetch_object($result))
  {
    $xlabel[] = $row->day;  
    $data[] = $row->ns_total;
    ++$count;
  }  
  $xlabel = array_reverse($xlabel);
  $data = array_reverse($data);
  Header("Content-Type: image/png");
  $ochart = new chart(500, 350,5, '#eeeeee');
  $ochart->setTitle("Total nicks, last month","#000000",2);
  $ochart->setPlotArea(SOLID,"#444444", '#dddddd');
  $ochart->setFormat(0,'.','.');
  $ochart->addSeries($data,'line','Nicks', SOLID,'#000000', '#0000ff');
  $ochart->setLabels($xlabel, '#000000', 1, VERTICAL);
  $ochart->setGrid("#bbbbbb", DASHED, "#bbbbbb", DOTTED);
  $ochart->plot('');
  //echo "</tr></table>";
  //echo"</td></tr></table></center>";  
  mysql_free_result($result);  
  mysql_close($link_id);
?>
