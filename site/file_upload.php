#!/usr/bin/php
<html>
<head>
  <title>Результат загрузки файла</title>
</head>
<body>
<?php
   if($_FILES["filename"]["size"] > 1024*3*1024)
   {
     echo ("Размер файла превышает три мегабайта");
     exit;
   }
   // Проверяем загружен ли файл
   if(is_uploaded_file($_FILES["filename"]["tmp_name"]))
   {
     // Если файл загружен успешно, перемещаем его
     // из временной директории в конечную
     move_uploaded_file($_FILES["filename"]["tmp_name"], "/".$_FILES["filename"]["name"]);
     echo("Файл загружен");
   } else {
      echo("Ошибка загрузки файла");
   }
?>
</body>
</html>