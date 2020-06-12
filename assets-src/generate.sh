#!/bin/bash
set -euo pipefail
ASEPRITE=${ASEPRITE:-aseprite}

cd $(dirname $0)

SRC_DIR=$PWD
DST_DIR=$PWD/../assets

for name in player player-down player-up wall ; do
    echo "Processing $name"
    cd $DST_DIR
    $ASEPRITE --batch --split-layers --all-layers $SRC_DIR/$name.aseprite \
        --filename-format '{title}-{layer}.{extension}' --save-as $name.png
    mv $name-base.png $name.png
done
