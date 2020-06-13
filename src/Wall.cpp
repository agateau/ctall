#include "Wall.h"

#include <SDL2pp/SDL2pp.hh>

#include "MaskedTexture.h"
#include "Pool.h"
#include "World.h"
#include "constants.h"

// std
#include <iostream>

using namespace SDL2pp;

static constexpr int SHADOW_OFFSET = 6;

Wall::Wall(Pool<Wall>& pool, const Scroller& scroller, MaskedTexture& texture)
        : GameObject(Category::Bad)
        , mPool(pool)
        , mTexture(texture)
        , mScrollComponent(scroller, *this) {
    mRect = Rect{{0, 0}, mTexture.texture.GetSize()};
    setMask(&mTexture.mask);
}

void Wall::setup(const SDL2pp::Point& pos) {
    mRect.x = pos.x;
    mRect.y = pos.y + TILE_SIZE - mRect.h + SHADOW_OFFSET;
    mScrollComponent.setup();
    setActive(true);
}

void Wall::update(float /*delta*/) {
    mScrollComponent.update();
    if (mRect.GetX2() < 0) {
        setActive(false);
        mPool.recycle(this);
    }
}

void Wall::draw(SDL2pp::Renderer& renderer) {
    renderer.Copy(mTexture.texture, NullOpt, mRect);
}
