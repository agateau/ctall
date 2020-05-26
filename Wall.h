#ifndef WALL_H
#define WALL_H

#include "GameObject.h"
#include "ScrollComponent.h"

#include <SDL2pp/SDL2pp.hh>

class Wall : public GameObject {
public:
    Wall(Pool<Wall>& pool, const Scroller& scroller, SDL2pp::Texture& texture)
        : mPool(pool), mTexture(texture), mScrollComponent(scroller, mPos)
    {
    }

    void setup(int row) {
        mPos.x = SCREEN_WIDTH;
        mPos.y = SCREEN_HEIGHT / 2 + row * ROW_HEIGHT;
        mScrollComponent.setup();
    }

    void update(float delta) override {
        mScrollComponent.update();
        if (mPos.x + ROW_HEIGHT < 0) {
            mPool.recycle(this);
        }
    }

    void draw(SDL2pp::Renderer& renderer) override {
        renderer.Copy(mTexture, SDL2pp::NullOpt, mPos);
    }

private:
    Pool<Wall>& mPool;
    SDL2pp::Texture& mTexture;
    ScrollComponent mScrollComponent;

    SDL2pp::Point mPos;
};

#endif /* WALL_H */
