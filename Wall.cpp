#include "Wall.h"

#include "constants.h"
#include "Pool.h"

#include <SDL2pp/SDL2pp.hh>

Wall::Wall(Pool<Wall>& pool, const Scroller& scroller, SDL2pp::Texture& texture)
    : mPool(pool), mTexture(texture), mScrollComponent(scroller, mPos)
{
}

void Wall::setup(int row) {
    mPos.x = SCREEN_WIDTH;
    mPos.y = SCREEN_HEIGHT / 2 + row * ROW_HEIGHT;
    mScrollComponent.setup();
}

void Wall::update(float delta) {
    mScrollComponent.update();
    if (mPos.x + ROW_HEIGHT < 0) {
        mPool.recycle(this);
    }
}

void Wall::draw(SDL2pp::Renderer& renderer) {
    renderer.Copy(mTexture, SDL2pp::NullOpt, mPos);
}
