#!/bin/bash


for file in "$@"
do
    source=$file
    dest="`dirname $source`/`basename $source .mp4`.webm"
    echo "do webm from $source to $dest"
    avconv -y -i $source -filter:v scale=640:420 -pix_fmt yuv420p  -b:v 500k -r:v 25/1 -threads 8 -force_fps  $dest
done
