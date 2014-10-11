#!/bin/bash


export MOVIE_PATH="/mnt/multimedia_b/_moviescan/movies"
export RAW_PATH="/mnt/multimedia_b/_moviescan/raw/"

export OPTS_IMAGES="--jpeg 100 -r 1600x1200 -p YUYV -q --no-banner -F 3"
export OPTS_AVCONV=" -f image2pipe -s 1600x1200 -vcodec mjpeg -r 18 -i - -vf transpose=1,transpose=0 -pix_fmt rgb24 -s 1600x1200 -r 25 -an -c:v mpeg4 -q:v 5 "


function proc_images {
    for f in $(ls $1/*.raw) ; 
    do 
        fswebcam -d RAW:$f $OPTS_IMAGES - ; 
    done;
}

function proc {
    echo "execute movie at $1" 
    rawpath=`dirname $1`
    movie=`basename $rawpath`.mp4
    rm "$rawpath/ready"
    (proc_images $rawpath) | avconv $OPTS_AVCONV $rawpath/$movie
    newpath="$MOVIE_PATH/`basename $rawpath`"
    mkdir $newpath
    mv $rawpath/$movie $newpath && date +"%Y%m%d" > $rawpath/done
    ~moviescan/scripts/2webm.sh "$newpath/$movie"
    ~moviescan/scripts/2mp4.sh "$newpath/$movie"
}


export -f proc
export -f proc_images
find $RAW_PATH -name ready -exec bash -c 'proc "{}"' \;

#find . -name "*.raw" -maxdepth 1 -exec fswebcam -d RAW:{}  $OPTS_IMAGES - \; | cat

#echo `for f in *.raw ; do fswebcam -d RAW:$f  $OPTS_IMAGES - ; done;` | avconv $OPTS_AVCONV

