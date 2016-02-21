<?php 
  require("../includes/config.inc.php");
  require("../includes/session.inc.php");   
?>
<?php 
  $upd = $_POST['upd'];
  $photoFileName = $_FILES['photo']['name']; // get client side file name 
  if ($photoFileName) // file uploaded 
    { 
      $fileNameParts = explode(".", $photoFileName); 
      $fileExtension = end($fileNameParts); // part behind last dot 
      if ($fileExtension != "jpg" && 
          $fileExtension != "JPEG" && 
          $fileExtension != "JPG") 
            { 
              die ("Choose a JPG for the photo"); 
            } 
      $photoSize = $_FILES['photo']['size']; // size of uploaded file 
      if ($photoSize == 0) 
        { 
          die ("Sorry. The upload of $photoFileName has failed. Search a photo smaller than 100K, using the button."); 
        } 
      if ($photoSize > 102400) 
        { 
          die ("Sorry. The file $photoFileName is larger than 100K. Advice: reduce the photo using a drawing tool."); 
        } 
        
      // read photo 
      $tempFileName = $_FILES['photo']['tmp_name']; // temporary file at server side 
      $image = file_get_contents ($tempFileName);
      
      // create thumb
      list($width, $height) = getimagesize($tempFileName);
      $new_width = 50; $new_height=50;
      $image_p = imagecreatetruecolor($new_width, $new_height);
      $image_o = imagecreatefromjpeg($tempFileName);
      imagecopyresampled($image_p, $image_o, 0, 0, 0, 0, $new_width, $new_height, $width, $height);
      ob_start(); // Start capturing stdout.
      imageJPEG($image_p, null, 100);
      $thumb = ob_get_contents(); // the raw jpeg image data.
      ob_end_clean(); // Dump the stdout so it does not screw other output.
      
      $link_id = db_connect();      
      if($upd == 0)
        {
          $query  =  "INSERT INTO ns_photo VALUES(0,".$_SESSION["user"]->snid;
          $query .= ", NOW(), 'P','".mysql_real_escape_string ($image)."',";
          $query .= "'".mysql_real_escape_string ($thumb)."', 0)";
        }
      else
        {
          $query  =  "UPDATE ns_photo ";
          $query .=  "SET photo='".mysql_real_escape_string ($image)."'";
          $query .=  " , thumb='".mysql_real_escape_string ($thumb)."'";
          $query .=  " , t_update=NOW() ";
          $query .=  " WHERE snid=".$snid;
        }
          
      mysql_query($query, $link_id) or die("There was an error inserting/updating the photo!");      
      mysql_close($link_id);        
      echo "Photo was uploaded.<br>";
  }
?>
<script language="JavaScript">
    parent.location = "index.php"
</script>
