#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

namespace SDL2pp {
class Renderer;
}

class GameObject {
public:
    virtual ~GameObject();
    virtual void update(float elapsed) = 0;
    virtual void draw(SDL2pp::Renderer& renderer) = 0;
};


#endif /* GAMEOBJECT_H */