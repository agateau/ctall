#!/bin/bash
set -euo pipefail
ASEPRITE=${ASEPRITE:-aseprite}

PLAYER_COUNT=2

cd $(dirname $0)

SRC_DIR=$PWD
DST_DIR=$PWD/../assets

for name in player player-down player-up wall ; do
    echo "Processing $name"
    cd $DST_DIR
    $ASEPRITE --batch --split-layers --all-layers $SRC_DIR/$name.aseprite \
        --filename-format '{title}-{layer}.{extension}' --save-as $name.png
    if [ -e $name-base.png ] ; then
        mv $name-base.png $name.png
    fi
done

for name in player player-down player-up ; do
    echo "Post-processing $name"
    cd $DST_DIR
    for idx in $(seq 0 $((PLAYER_COUNT-1))) ; do
        cp $name-mask.png $name-$idx-mask.png
        mv $name-base-$idx.png $name-$idx.png
    done
    rm $name-mask.png
done
