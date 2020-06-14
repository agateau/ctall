#!/bin/bash
set -euo pipefail

cd $(dirname $0)

SRC_DIR=$PWD
TOOLS_DIR=$PWD
DST_DIR=$PWD/../assets

main() {
    for name in player player-down player-up ; do
        $TOOLS_DIR/oralayers2png $SRC_DIR/$name.ora \
            base-0=$DST_DIR/$name-0.png \
            base-1=$DST_DIR/$name-1.png \
            mask=$DST_DIR/$name-0-mask.png \
            mask=$DST_DIR/$name-1-mask.png
    done

    $TOOLS_DIR/oralayers2png $SRC_DIR/wall.ora \
        base=$DST_DIR/wall.png \
        mask=$DST_DIR/wall-mask.png
}

main
