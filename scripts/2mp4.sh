#!/bin/bash


for file in "$@"
do
    source=$file
    dest_dir="`dirname $source`"
    dest="`dirname $source`/`basename $source .mp4`.web.mp4"
    echo "do mp4 for web from $source to $dest"
    avconv -y -i $source -threads 8 -vcodec libx264  -s 640:420  -pix_fmt yuv420p -c:v libx264 -preset:v slow -b:v 700k -r:v 25/1 -force_fps -f mp4  $dest_dir/temp.mp4
    MP4Box -add $dest_dir/temp.mp4 $dest
    rm $dest_dir/temp.mp4
    echo "done :-)"
done
