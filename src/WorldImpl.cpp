#include "WorldImpl.h"

#include "Assets.h"
#include "Random.h"

static constexpr int SECTION_COUNT = 10;
static constexpr int MIN_SECTION_LENGTH = 4;
static constexpr int MAX_SECTION_LENGTH = 15;

static constexpr int SCORE_ROUND = 100;
static constexpr int SCORE_PER_CAPTURE = 1000;

using namespace Random;

WorldImpl::WorldImpl(Assets& assets, const Input& input)
        : mScroller(*this)
        , mBackground(*this, mScroller, *this)
        , mPlayer(*this, assets.player, assets.playerUp, assets.playerDown, input)
        , mWallPool([this, &assets]() {
            return new Wall(*this, mWallPool, mScroller, assets.wall);
        })
        , mBonusPool([this, &assets]() {
            return new Bonus(*this, mBonusPool, mScroller, assets.bonuses);
        })
        , mAssets(assets) {
    mGameObjects.push_back(&mPlayer);

    createSections();
}

/**
 * At one point sections will be created from assets
 */
void WorldImpl::createSections() {
    for (int i = 0; i < SECTION_COUNT; ++i) {
        std::vector<Section::Column> columns;
        int length = randomRange(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH);
        const auto& assets = randomChoice(mAssets.backgrounds);
        for (int columnIdx = 0; columnIdx < length; ++columnIdx) {
            Section::Column column(MAX_LANE - MIN_LANE + 1 + 2); // + 2 for borders
            auto it = column.begin();
            auto last = column.end() - 1;
            *it = &assets.border;
            ++it;
            for (; it != last; ++it) {
                *it = &Random::randomChoice(assets.roads);
            }
            *last = &assets.border;
            columns.push_back(column);
        }
        mSections.emplace_back(Section{columns});
    }
}

void WorldImpl::spawnThings() {
    int wallLane = MIN_LANE - 1;
    if (randomBool() == 0) {
        auto wall = mWallPool.get();
        wallLane = randomRange(MIN_LANE, MAX_LANE + 1);
        wall->setup(wallLane);
        mGameObjects.push_back(wall);
    }

    if (randomRange(4) == 0) {
        auto bonus = mBonusPool.get();
        int bonusLane = wallLane;
        while (bonusLane == wallLane) {
            bonusLane = randomRange(MIN_LANE, MAX_LANE + 1);
        }
        bonus->setup(bonusLane);
        mGameObjects.push_back(bonus);
    }
}

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
    return SCREEN_HEIGHT / 2 + lane * LANE_WIDTH - LANE_WIDTH / 2;
}

const Section* WorldImpl::getSection() const {
    return &randomChoice(mSections);
}
