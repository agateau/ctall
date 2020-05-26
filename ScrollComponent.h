#ifndef SCROLLCOMPONENT_H
#define SCROLLCOMPONENT_H

namespace SDL2pp {
class Point;
}

class Scroller;

class ScrollComponent {
public:
    ScrollComponent(const Scroller& scroller, SDL2pp::Point& point);
    void setup();
    void update();

private:
    const Scroller& mScroller;
    SDL2pp::Point& mPoint;
    float mStartX = 0;
};

#endif /* SCROLLCOMPONENT_H */
