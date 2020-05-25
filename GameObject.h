#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class Renderer;

class GameObject {
public:
    virtual ~GameObject();
    virtual void update(float elapsed) = 0;
    virtual void draw(const Renderer& renderer) = 0;
};


#endif /* GAMEOBJECT_H */
