#ifndef SCROLLCOMPONENT_H
#define SCROLLCOMPONENT_H

#include <SFML/Graphics.hpp>

class Scroller;

class ScrollComponent {
public:
    ScrollComponent(const Scroller& scroller, sf::Sprite& sprite);
    void setup();
    void update();

private:
    const Scroller& mScroller;
    sf::Sprite& mSprite;
    float mStartX = 0;
};

#endif /* SCROLLCOMPONENT_H */
