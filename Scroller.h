#ifndef SCROLLER_H
#define SCROLLER_H

#include "constants.h"

class Scroller {
public:
    class Listener {
    public:
        virtual ~Listener();
        virtual void spawnThings() = 0;
    };

    Scroller(Listener& listener);

    void update(float delta);

    float getPosition() const;

private:
    void accelerate();

    Listener& mListener;
    float mPos = 0;
    float mScrollSpeed = START_SCROLL_SPEED;
};

#endif /* SCROLLER_H */
