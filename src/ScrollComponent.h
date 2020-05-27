#ifndef SCROLLCOMPONENT_H
#define SCROLLCOMPONENT_H

class GameObject;
class Scroller;

class ScrollComponent {
public:
    ScrollComponent(const Scroller& scroller, GameObject& gameObject);
    void setup();
    void update();

private:
    const Scroller& mScroller;
    GameObject& mGameObject;
    int mStartX = 0;
};

#endif /* SCROLLCOMPONENT_H */
