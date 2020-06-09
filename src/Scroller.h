#ifndef SCROLLER_H
#define SCROLLER_H

#include "constants.h"

class Scroller {
public:
    Scroller() = default;
    Scroller(const Scroller&) = delete;
    void update(float delta);

    float getPosition() const;

private:
    float mPos = 0;
    float mScrollSpeed = START_SCROLL_SPEED;
};

#endif /* SCROLLER_H */
