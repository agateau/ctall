#ifndef WALL_H
#define WALL_H

#include "GameObject.h"
#include "ScrollComponent.h"

#include <SDL2pp/SDL2pp.hh>

template<class T>
class Pool;

class Wall : public GameObject {
public:
    Wall(Pool<Wall>& pool, const Scroller& scroller, SDL2pp::Texture& texture);

    void setup(int row);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

private:
    Pool<Wall>& mPool;
    SDL2pp::Texture& mTexture;
    ScrollComponent mScrollComponent;

    SDL2pp::Point mPos;
};

#endif /* WALL_H */
