<?php

header('Content-type: image/jpeg');

$DIR=realpath('../movies/raw');
$CMD1 = 'fswebcam -d RAW:';
$CMD2 = ' -r 1600x1200 --jpeg 90 --rotate 180 --flip h --scale 900x675 -F 1 -S 1 -p YUYV ';


if ($handle = opendir($DIR)) {

    $newest = null;
    $newest_time = 0;
    foreach(glob($DIR.'/*', GLOB_ONLYDIR) as $entry) {
        if ($newest_time < filemtime($entry.'/current')){
            $newest = $entry.'/current';
            $newest_time = filemtime($newest);
        }
    
    
    }
    
    echo shell_exec($CMD1.$newest.$CMD2.' --title "Snapshot from: '. date("d.m.Y H:i:s",filemtime($newest)).'"  -');
    flush();
}


?>
