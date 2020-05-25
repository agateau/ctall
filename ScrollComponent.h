#ifndef SCROLLCOMPONENT_H
#define SCROLLCOMPONENT_H

#include <SDL2/SDL.h>

class Scroller;

class ScrollComponent {
public:
    ScrollComponent(const Scroller& scroller, SDL_Point& point);
    void setup();
    void update();

private:
    const Scroller& mScroller;
    SDL_Point& mPoint;
    float mStartX = 0;
};

#endif /* SCROLLCOMPONENT_H */
