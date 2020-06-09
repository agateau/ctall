#include "Scroller.h"

#include <iostream>

void Scroller::update(float delta) {
    float oldPos = mPos;
    mPos += mScrollSpeed * delta;
    int oldLevel = int(oldPos / LEVEL_LENGTH);
    int level = int(mPos / LEVEL_LENGTH);
    if (level > oldLevel) {
        mScrollSpeed *= LEVEL_SPEEDUP;
    }
}

float Scroller::getPosition() const {
    return mPos;
}
