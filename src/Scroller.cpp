#include "Scroller.h"

Scroller::Listener::~Listener() = default;

Scroller::Scroller(Scroller::Listener& listener) : mListener(listener) {
}

void Scroller::update(float delta) {
    float oldPos = mPos;
    mPos += mScrollSpeed * delta;
    if (int(mPos) % SPAWN_SPACING < int(oldPos) % SPAWN_SPACING) {
        mListener.spawnThings();
    }
    int oldLevel = int(oldPos / LEVEL_LENGTH);
    int level = int(mPos / LEVEL_LENGTH);
    if (level > oldLevel) {
        mScrollSpeed *= LEVEL_SPEEDUP;
        mListener.onLevelChanged(level);
    }
}

float Scroller::getPosition() const {
    return mPos;
}
