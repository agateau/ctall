#ifndef WALL_H
#define WALL_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"
#include "ScrollComponent.h"

class Game;

template <class T> class Pool;

class Wall : public GameObject {
public:
    Wall(Game& game, Pool<Wall>& pool, const Scroller& scroller, SDL2pp::Texture& texture);

    void setup(int row);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

private:
    Game& mGame;
    Pool<Wall>& mPool;
    SDL2pp::Texture& mTexture;
    ScrollComponent mScrollComponent;
};

#endif /* WALL_H */
