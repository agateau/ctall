#include "Bonus.h"

#include <SDL2pp/SDL2pp.hh>

#include "Pool.h"
#include "Random.h"
#include "World.h"
#include "constants.h"

using namespace SDL2pp;

Bonus::Bonus(Pool<Bonus>& pool,
             const Scroller& scroller,
             std::vector<MaskedTexture>& textures)
        : GameObject(Category::Bonus)
        , mPool(pool)
        , mTextures(textures)
        , mScrollComponent(scroller, *this) {
}

void Bonus::setup(const Point& pos) {
    mActiveTexture = &Random::randomChoice(mTextures);
    setMask(&mActiveTexture->mask);
    auto size = mActiveTexture->texture.GetSize();

    // Update h before y to properly center the bonus
    mRect.w = size.x;
    mRect.h = size.y;
    mRect.x = pos.x + (TILE_SIZE - mRect.w) / 2;
    mRect.y = pos.y + (TILE_SIZE - mRect.h) / 2;
    mScrollComponent.setup();
    setActive(true);
}

void Bonus::update(float /*delta*/) {
    mScrollComponent.update();
    if (mRect.GetX2() < 0) {
        setActive(false);
        mPool.recycle(this);
    }
}

void Bonus::draw(SDL2pp::Renderer& renderer) {
    renderer.Copy(mActiveTexture->texture, NullOpt, mRect);
}

void Bonus::onCaptured() {
    setActive(false);
    mPool.recycle(this);
}
