#include "Wall.h"

#include <SDL2pp/SDL2pp.hh>

#include "Game.h"
#include "Pool.h"
#include "constants.h"

using namespace SDL2pp;

Wall::Wall(Game& game, Pool<Wall>& pool, const Scroller& scroller, Texture& texture)
        : mGame(game), mPool(pool), mTexture(texture), mScrollComponent(scroller, *this) {
    mRect = Rect{{0, 0}, mTexture.GetSize()};
}

void Wall::setup(int row) {
    mRect.x = SCREEN_WIDTH;
    mRect.y = mGame.yForLane(row) - mRect.h / 2;
    mScrollComponent.setup();
}

void Wall::update(float /*delta*/) {
    mScrollComponent.update();
    if (mRect.GetX2() < 0) {
        mPool.recycle(this);
    }
}

void Wall::draw(SDL2pp::Renderer& renderer) {
    renderer.Copy(mTexture, NullOpt, mRect);
}
