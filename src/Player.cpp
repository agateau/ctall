#include "Player.h"

#include "Game.h"
#include "Input.h"
#include "constants.h"

using namespace SDL2pp;

static constexpr float MOVE_SPEED = 400;

Player::Player(Game& game, Texture& texture, const Input& input)
        : mGame(game), mTexture(texture), mInput(input) {
    auto point = mTexture.GetSize();
    mRect.w = point.x;
    mRect.h = point.y;
    mRect.x = 12;
    mRect.y = mGame.yForLane(mCurrentLane) - mRect.h / 2;
}

void Player::update(float delta) {
    if (mInput.down && mTargetLane < MAX_LANE && mTargetLane == mCurrentLane) {
        ++mTargetLane;
    }
    if (mInput.up && mTargetLane > MIN_LANE && mTargetLane == mCurrentLane) {
        --mTargetLane;
    }

    int targetY = mGame.yForLane(mTargetLane) - mRect.h / 2;

    if (mTargetLane < mCurrentLane) {
        mRect.y = std::max(targetY, mRect.y - int(MOVE_SPEED * delta));
    } else if (mTargetLane > mCurrentLane) {
        mRect.y = std::min(targetY, mRect.y + int(MOVE_SPEED * delta));
    }
    if (mRect.y == targetY) {
        mCurrentLane = mTargetLane;
    }
}

void Player::draw(Renderer& renderer) {
    renderer.Copy(mTexture, NullOpt, mRect);
}
