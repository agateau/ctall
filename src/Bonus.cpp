#include "Bonus.h"

#include <SDL2pp/SDL2pp.hh>

#include "Game.h"
#include "Pool.h"
#include "constants.h"

using namespace SDL2pp;

static int randint(int min, int max) {
    int random = std::rand();
    return min + random % (max - min + 1);
}

Bonus::Bonus(Game& game,
             Pool<Bonus>& pool,
             const Scroller& scroller,
             std::vector<Texture>& textures)
        : GameObject(Category::Bonus)
        , mGame(game)
        , mPool(pool)
        , mTextures(textures)
        , mScrollComponent(scroller, *this) {
}

void Bonus::setup(int lane) {
    mRect.x = SCREEN_WIDTH;
    mRect.y = mGame.yForLane(lane) - mRect.h / 2;
    mActiveTexture = &mTextures.at(randint(0, int(mTextures.size()) - 1));
    auto size = mActiveTexture->GetSize();
    mRect.w = size.x;
    mRect.h = size.y;
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
    renderer.Copy(*mActiveTexture, NullOpt, mRect);
}

void Bonus::onCaptured() {
    setActive(false);
    mPool.recycle(this);
}
