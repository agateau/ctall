#ifndef WALL_H
#define WALL_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"
#include "ScrollComponent.h"

class GameScreen;
class MaskedTexture;

template <class T> class Pool;

class Wall : public GameObject {
public:
    Wall(GameScreen& game, Pool<Wall>& pool, const Scroller& scroller, MaskedTexture& texture);

    void setup(int lane);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

private:
    GameScreen& mGameScreen;
    Pool<Wall>& mPool;
    MaskedTexture& mTexture;
    ScrollComponent mScrollComponent;
};

#endif /* WALL_H */
