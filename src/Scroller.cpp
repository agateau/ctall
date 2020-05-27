#include "Scroller.h"

Scroller::Listener::~Listener() = default;

Scroller::Scroller(Scroller::Listener& listener) : mListener(listener) {
}

void Scroller::update(float delta) {
    float oldPos = mPos;
    mPos += mScrollSpeed * delta;
    if (int(mPos) % SPAWN_SPACING < int(oldPos) % SPAWN_SPACING) {
        mListener.spawnThings();
        accelerate();
    }
}

float Scroller::getPosition() const {
    return mPos;
}

void Scroller::accelerate() {
    mScrollSpeed *= SCROLL_SPEED_MULTIPLIER;
}
