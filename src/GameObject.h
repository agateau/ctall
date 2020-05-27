#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2pp/SDL2pp.hh>

class GameObject {
public:
    virtual ~GameObject();
    virtual void update(float elapsed) = 0;
    virtual void draw(SDL2pp::Renderer& renderer) = 0;

    const SDL2pp::Rect& rect() const {
        return mRect;
    }

    SDL2pp::Rect& rect() {
        return mRect;
    }

    bool isActive() const;

protected:
    void setActive(bool active);

    SDL2pp::Rect mRect;

private:
    bool mActive = true;
};


#endif /* GAMEOBJECT_H */
