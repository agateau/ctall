#include "WorldImpl.h"

#include "Assets.h"
#include "Bonus.h"
#include "Pool.h"
#include "Random.h"
#include "Wall.h"

static constexpr int SCORE_ROUND = 100;
static constexpr int SCORE_PER_CAPTURE = 1000;

using namespace std;
using namespace Random;
using namespace SDL2pp;

class WallTrigger : public Trigger {
public:
    WallTrigger(Assets& assets, const Scroller& scroller)
            : mPool(
                [this, &scroller, &assets]() { return new Wall(mPool, scroller, assets.wall); }) {
    }

    ~WallTrigger() override = default;

    void exec(World& world, const Point& pos) const override {
        auto wall = mPool.get();
        wall->setup(pos);
        world.addGameObject(wall);
    }

    mutable Pool<Wall> mPool;
};

class BonusTrigger : public Trigger {
public:
    BonusTrigger(Assets& assets, const Scroller& scroller)
            : mPool([this, &scroller, &assets]() {
                return new Bonus(mPool, scroller, assets.bonuses);
            }) {
    }

    ~BonusTrigger() override = default;

    void exec(World& world, const Point& pos) const override {
        auto bonus = mPool.get();
        bonus->setup(pos);
        world.addGameObject(bonus);
    }

private:
    mutable Pool<Bonus> mPool;
};

WorldImpl::WorldImpl(Assets& assets,
                     const Input& input,
                     const SDL2pp::Point& screenSize,
                     int playerIdx)
        : mScreenSize(screenSize)
        , mWallTrigger(make_unique<WallTrigger>(assets, mScroller))
        , mBonusTrigger(make_unique<BonusTrigger>(assets, mScroller))
        , mTriggers(
              {{TriggerId::Wall, mWallTrigger.get()}, {TriggerId::Bonus, mBonusTrigger.get()}})
        , mBackground(*this, mScroller, *this, mTriggers, mScreenSize)
        , mPlayer(*this, assets.players.at(playerIdx), input)
        , mAssets(assets) {
    mGameObjects.push_back(&mPlayer);

    mStartSections = {
        mAssets.sectionsByName.at("start"),
        mAssets.sectionsByName.at("futuristic-1"),
        mAssets.sectionsByName.at("futuristic-2"),
        mAssets.sectionsByName.at("blm"),
        mAssets.sectionsByName.at("futuristic-2"),
        mAssets.sectionsByName.at("futuristic-1"),
        mAssets.sectionsByName.at("futuristic-2"),
    };
}

WorldImpl::~WorldImpl() = default;

void WorldImpl::update(float delta) {
    mScroller.update(delta);
    mBackground.update();
    // Do not use an iterator here since it might become invalid if mGameObjects is resized after
    // an addition
    for (int idx = int(mGameObjects.size() - 1); idx >= 0; --idx) {
        mGameObjects.at(idx)->update(delta);
    }
    mGameObjects.erase(std::remove_if(mGameObjects.begin(),
                                      mGameObjects.end(),
                                      [](GameObject* object) { return !object->isActive(); }),
                       mGameObjects.end());

    // Ensure game objects are drawn in the right order
    std::sort(mGameObjects.begin(), mGameObjects.end(), [](const auto* o1, const auto* o2) {
        return o1->rect().y < o2->rect().y;
    });
}

int WorldImpl::score() const {
    int distance = int(mScroller.getPosition() / SCORE_ROUND) * SCORE_ROUND;
    return distance + mPlayer.capturedCount() * SCORE_PER_CAPTURE;
}

WorldImpl::State WorldImpl::state() const {
    return mState;
}

void WorldImpl::switchToPauseState() {
    mState = State::Paused;
}

void WorldImpl::switchToRunningState() {
    mState = State::Running;
}

void WorldImpl::switchToGameOverState() {
    mState = State::GameOver;
}

int WorldImpl::yForLane(int lane) const {
    const int roadPixelHeight = LANE_COUNT * TILE_SIZE;
    return (mScreenSize.y - roadPixelHeight) / 2 + lane * TILE_SIZE;
}

void WorldImpl::addGameObject(GameObject* gameObject) {
    mGameObjects.push_back(gameObject);
}

const Section* WorldImpl::getSection() {
    if (mStartSectionIdx < mStartSections.size()) {
        return mStartSections.at(mStartSectionIdx++);
    } else {
        return randomChoice(mAssets.sections).get();
    }
}
