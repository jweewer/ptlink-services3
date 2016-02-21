<?php
  session_start();
  //session_regenerate_id();
  $m_image = imagecreate (70, 25);
  $fillColor = imagecolorallocate($m_image, 200, 200, 200);
  imagefilledrectangle($m_image, 0, 0, 70, 25, $fillColor);
  $textColor = imagecolorallocate($m_image, 0, 0, 255);
  // Put test
  imagestring($m_image, 7, 5, 5, $_SESSION["regcode"], $textColor);
  // Dumpt content
  Header("Content-Type: image/png");
  imagepng($m_image);
?>
