#include "Player.h"

#include <algorithm>

#include "Bonus.h"
#include "Input.h"
#include "MaskedTexture.h"
#include "World.h"
#include "constants.h"

using namespace SDL2pp;

static constexpr float MOVE_SPEED = 200;

// Distance from top of sprite to top of board
static constexpr int OFFSET_UP = 15;

// Distance from top of sprite to bottom of board
static constexpr int OFFSET_DOWN = 20;

Player::Player(World& world,
               MaskedTexture& texture,
               MaskedTexture& upTexture,
               MaskedTexture& downTexture,
               const Input& input)
        : GameObject(Category::Player)
        , mWorld(world)
        , mTexture(texture)
        , mUpTexture(upTexture)
        , mDownTexture(downTexture)
        , mInput(input) {
    setActiveTexture(&mTexture);
    mRect.x = 12;
    mRect.y = mWorld.yForLane(0) + (TILE_SIZE - mRect.h) / 2;
}

void Player::update(float delta) {
    updateY(delta);
    updateTexture();
    checkCollisions();
}

void Player::draw(Renderer& renderer) {
    renderer.Copy(mActiveTexture->texture, NullOpt, mRect);
}

void Player::updateY(float delta) {
    const int minY = mWorld.yForLane(0) - OFFSET_UP;
    const int maxY = mWorld.yForLane(LANE_COUNT) - OFFSET_DOWN;
    if (mInput.down && mRect.y < maxY) {
        mRect.y = std::min(int(mRect.y + MOVE_SPEED * delta), maxY);
    }
    if (mInput.up && mRect.y > minY) {
        mRect.y = std::max(int(mRect.y - MOVE_SPEED * delta), minY);
    }
}

void Player::updateTexture() {
    if (mInput.down) {
        setActiveTexture(&mDownTexture);
    } else if (mInput.up) {
        setActiveTexture(&mUpTexture);
    } else {
        setActiveTexture(&mTexture);
    }
}

void Player::checkCollisions() {
    auto it = mWorld.gameObjects().begin();
    auto end = mWorld.gameObjects().end();
    for (;; ++it) {
        it = GameObject::findCollision(*this, it, end);
        if (it == end) {
            break;
        }
        auto* object = *it;
        auto category = object->category();
        if (category == GameObject::Category::Bad) {
            mWorld.switchToGameOverState();
            return;
        } else if (category == GameObject::Category::Bonus) {
            auto bonus = static_cast<Bonus*>(object);
            ++mCapturedCount;
            bonus->onCaptured();
        }
    }
}

void Player::setActiveTexture(MaskedTexture* texture) {
    if (mActiveTexture == texture) {
        return;
    }
    assert(texture);
    mActiveTexture = texture;
    auto size = texture->texture.GetSize();
    mRect.w = size.x;
    mRect.h = size.y;
    setMask(&mActiveTexture->mask);
}
