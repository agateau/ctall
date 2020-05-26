#include "Wall.h"

#include <SDL2pp/SDL2pp.hh>

#include "Game.h"
#include "Pool.h"
#include "constants.h"

Wall::Wall(Game& game, Pool<Wall>& pool, const Scroller& scroller, SDL2pp::Texture& texture)
        : mGame(game), mPool(pool), mTexture(texture), mScrollComponent(scroller, mPos) {
    mHeight = mTexture.GetHeight();
}

void Wall::setup(int row) {
    mPos.x = SCREEN_WIDTH;
    mPos.y = mGame.yForLane(row) - mHeight / 2;
    mScrollComponent.setup();
}

void Wall::update(float /*delta*/) {
    mScrollComponent.update();
    if (mPos.x + LANE_WIDTH < 0) {
        mPool.recycle(this);
    }
}

void Wall::draw(SDL2pp::Renderer& renderer) {
    renderer.Copy(mTexture, SDL2pp::NullOpt, mPos);
}
