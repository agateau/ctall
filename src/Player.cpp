#include "Player.h"

#include "Bonus.h"
#include "GameScreen.h"
#include "Input.h"
#include "constants.h"

using namespace SDL2pp;

static constexpr float MOVE_SPEED = 400;

Player::Player(GameScreen& gameScreen, Texture& texture, const Input& input)
        : GameObject(Category::Player), mGameScreen(gameScreen), mTexture(texture), mInput(input) {
    auto point = mTexture.GetSize();
    mRect.w = point.x;
    mRect.h = point.y;
    mRect.x = 12;
    mRect.y = mGameScreen.yForLane(mCurrentLane) - mRect.h / 2;
}

void Player::update(float delta) {
    updateTargetLane();
    updateY(delta);
    checkCollisions();
}

void Player::draw(Renderer& renderer) {
    renderer.Copy(mTexture, NullOpt, mRect);
}

void Player::updateTargetLane() {
    if (mInput.down && mTargetLane < MAX_LANE && mTargetLane == mCurrentLane) {
        ++mTargetLane;
    }
    if (mInput.up && mTargetLane > MIN_LANE && mTargetLane == mCurrentLane) {
        --mTargetLane;
    }
}

void Player::updateY(float delta) {
    int targetY = mGameScreen.yForLane(mTargetLane) - mRect.h / 2;

    if (mTargetLane < mCurrentLane) {
        mRect.y = std::max(targetY, mRect.y - int(MOVE_SPEED * delta));
    } else if (mTargetLane > mCurrentLane) {
        mRect.y = std::min(targetY, mRect.y + int(MOVE_SPEED * delta));
    }
    if (mRect.y == targetY) {
        mCurrentLane = mTargetLane;
    }
}

template <typename Iterator>
static bool collide(const GameObject& object, Iterator first, Iterator last) {
    auto rect = object.rect();
    auto hit = std::find_if(
        first, last, [&rect](GameObject* other) { return rect.Intersects(other->rect()); });
    return hit != last;
}

static bool collide(const GameObject* o1, const GameObject* o2) {
    return o1->rect().Intersects(o2->rect());
}

void Player::checkCollisions() {
    for (auto* object : mGameScreen.activeGameObjects()) {
        if (object == this) {
            continue;
        }
        if (!collide(this, object)) {
            continue;
        }
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
