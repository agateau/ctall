#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

class GameObject {
public:
    virtual ~GameObject();
    virtual void update(sf::Time elapsed) = 0;
    virtual void draw(sf::RenderTarget& target) = 0;
};


#endif /* GAMEOBJECT_H */
