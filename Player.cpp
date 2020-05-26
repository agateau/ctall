#include "Player.h"

#include "Game.h"
#include "Input.h"
#include "constants.h"

using namespace SDL2pp;

static constexpr float MOVE_SPEED = 400;

Player::Player(Game& game, Texture& texture, const Input& input)
        : mGame(game), mTexture(texture), mInput(input) {
    mHeight = mTexture.GetHeight();
    mPos.x = 12;
    mPos.y = mGame.yForLane(mCurrentLane) - mHeight / 2;
}

void Player::update(float delta) {
    if (mInput.down && mTargetLane < MAX_LANE && mTargetLane == mCurrentLane) {
        ++mTargetLane;
    }
    if (mInput.up && mTargetLane > MIN_LANE && mTargetLane == mCurrentLane) {
        --mTargetLane;
    }

    int targetY = mGame.yForLane(mTargetLane) - mHeight / 2;

    if (mTargetLane < mCurrentLane) {
        mPos.y = std::max(targetY, mPos.y - int(MOVE_SPEED * delta));
    } else if (mTargetLane > mCurrentLane) {
        mPos.y = std::min(targetY, mPos.y + int(MOVE_SPEED * delta));
    }
    if (mPos.y == targetY) {
        mCurrentLane = mTargetLane;
    }
}

void Player::draw(Renderer& renderer) {
    renderer.Copy(mTexture, NullOpt, mPos);
}
