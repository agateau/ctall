#include "Player.h"

#include "Bonus.h"
#include "GameScreen.h"
#include "Input.h"
#include "MaskedTexture.h"
#include "constants.h"

#include <algorithm>

using namespace SDL2pp;

static constexpr float MOVE_SPEED = 200;

// Distance from top of sprite to top of board
static constexpr int OFFSET_UP = 15;

// Distance from top of sprite to bottom of board
static constexpr int OFFSET_DOWN = 20;

Player::Player(GameScreen& gameScreen, MaskedTexture& texture, const Input& input)
        : GameObject(Category::Player), mGameScreen(gameScreen), mTexture(texture), mInput(input) {
    auto point = mTexture.texture.GetSize();
    mRect.w = point.x;
    mRect.h = point.y;
    mRect.x = 12;
    mRect.y = mGameScreen.yForLane(0) + (LANE_WIDTH - mRect.h) / 2;
    setMask(&mTexture.mask);
}

void Player::update(float delta) {
    updateY(delta);
    checkCollisions();
}

void Player::draw(Renderer& renderer) {
    renderer.Copy(mTexture.texture, NullOpt, mRect);
}

void Player::updateY(float delta) {
    const int minY = mGameScreen.yForLane(MIN_LANE) - OFFSET_UP;
    const int maxY = mGameScreen.yForLane(MAX_LANE + 1) - OFFSET_DOWN;
    if (mInput.down && mRect.y < maxY) {
        mRect.y = std::min(int(mRect.y + MOVE_SPEED * delta), maxY);
    }
    if (mInput.up && mRect.y > minY) {
        mRect.y = std::max(int(mRect.y - MOVE_SPEED * delta), minY);
    }
}

void Player::checkCollisions() {
    auto it = mGameScreen.activeGameObjects().begin();
    auto end = mGameScreen.activeGameObjects().end();
    for (;; ++it) {
        it = GameObject::findCollision(*this, it, end);
        if (it == end) {
            break;
        }
        auto* object = *it;
        auto category = object->category();
        if (category == GameObject::Category::Bad) {
            mGameScreen.switchToGameOverState();
            return;
        } else if (category == GameObject::Category::Bonus) {
            auto bonus = static_cast<Bonus*>(object);
            ++mCapturedCount;
            bonus->onCaptured();
        }
    }
}
