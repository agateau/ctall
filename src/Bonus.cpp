#include "Bonus.h"

#include <SDL2pp/SDL2pp.hh>

#include "Pool.h"
#include "Random.h"
#include "World.h"
#include "constants.h"

using namespace SDL2pp;

Bonus::Bonus(const World& world,
             Pool<Bonus>& pool,
             const Scroller& scroller,
             std::vector<MaskedTexture>& textures)
        : GameObject(Category::Bonus)
        , mWorld(world)
        , mPool(pool)
        , mTextures(textures)
        , mScrollComponent(scroller, *this) {
}

void Bonus::setup(int lane) {
    mActiveTexture = &Random::randomChoice(mTextures);
    setMask(&mActiveTexture->mask);
    auto size = mActiveTexture->texture.GetSize();

    // Update h before y to properly center the bonus
    mRect.w = size.x;
    mRect.h = size.y;
    mRect.x = SCREEN_WIDTH;
    mRect.y = mWorld.yForLane(lane) + (LANE_WIDTH - mRect.h) / 2;
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
