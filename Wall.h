#ifndef WALL_H
#define WALL_H

#include "GameObject.h"
#include "ScrollComponent.h"

class Wall : public GameObject {
public:
    Wall(Pool<Wall>& pool, const Scroller& scroller, SDL_Texture* texture)
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

    void draw(const Renderer& renderer) override {
        renderer.renderTexture(mTexture, mPos.x, mPos.y);
    }

private:
    Pool<Wall>& mPool;
    SDL_Texture* const mTexture;
    ScrollComponent mScrollComponent;

    SDL_Point mPos;
};

#endif /* WALL_H */
