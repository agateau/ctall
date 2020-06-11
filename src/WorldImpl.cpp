#include "WorldImpl.h"

#include "Assets.h"
#include "Bonus.h"
#include "Pool.h"
#include "Random.h"
#include "Wall.h"

static constexpr int SECTION_COUNT = 10;
static constexpr int MIN_SECTION_LENGTH = 4;
static constexpr int MAX_SECTION_LENGTH = 15;

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

    ~WallTrigger() = default;

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

    ~BonusTrigger() = default;

    void exec(World& world, const Point& pos) const override {
        auto bonus = mPool.get();
        bonus->setup(pos);
        world.addGameObject(bonus);
    }

private:
    mutable Pool<Bonus> mPool;
};

WorldImpl::WorldImpl(Assets& assets, const Input& input)
        : mBackground(*this, mScroller, *this)
        , mPlayer(*this, assets.player, assets.playerUp, assets.playerDown, input)
        , mAssets(assets)
        , mWallTrigger(make_unique<WallTrigger>(assets, mScroller))
        , mBonusTrigger(make_unique<BonusTrigger>(assets, mScroller)) {
    mGameObjects.push_back(&mPlayer);
    createSections();
}

WorldImpl::~WorldImpl() = default;

static Section loadSection(const vector<BackgroundAssets>& bgAssetsList,
                           Trigger* wallTrigger,
                           Trigger* bonusTrigger,
                           const vector<string>& lines) {
    ColumnArray<const SDL2pp::Texture*> images;
    ColumnArray<const Trigger*> triggers;

    assert(lines.size() == LANE_COUNT);
    auto columnCount = lines.front().size();
    vector<Section::Column> columns;

    const auto& assets = randomChoice(bgAssetsList);
    for (size_t columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
        auto it = images.begin();
        auto last = images.end() - 1;
        *it = &assets.border;
        ++it;
        for (; it != last; ++it) {
            *it = &Random::randomChoice(assets.roads);
        }
        *last = &assets.border;

        std::fill(triggers.begin(), triggers.end(), nullptr);
        for (size_t row = 0; row < lines.size(); ++row) {
            auto ch = lines.at(row).at(columnIdx);
            if (ch == '|') {
                triggers.at(row + 1) = wallTrigger;
            } else if (ch == '*') {
                triggers.at(row + 1) = bonusTrigger;
            }
        }
        columns.emplace_back(Section::Column{images, triggers});
    }
    return Section{columns};
}

/**
 * At one point sections will be created from assets
 */
void WorldImpl::createSections() {
    ColumnArray<const SDL2pp::Texture*> images;
    ColumnArray<const Trigger*> triggers;

    for (int i = 0; i < SECTION_COUNT; ++i) {
        std::vector<Section::Column> columns;
        int length = randomRange(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH);
        const auto& assets = randomChoice(mAssets.backgrounds);
        for (int columnIdx = 0; columnIdx < length; ++columnIdx) {
            auto it = images.begin();
            auto last = images.end() - 1;
            *it = &assets.border;
            ++it;
            for (; it != last; ++it) {
                *it = &Random::randomChoice(assets.roads);
            }
            *last = &assets.border;

            std::fill(triggers.begin(), triggers.end(), nullptr);
            if (columnIdx % SPAWN_SPACING == 0) {
                fillTriggers(triggers);
            }

            columns.emplace_back(Section::Column{images, triggers});
        }
        mSections.emplace_back(Section{columns});
    }
    mSections.emplace_back(loadSection(mAssets.backgrounds,
                                       mWallTrigger.get(),
                                       mBonusTrigger.get(),
                                       {
                                           "||     ",
                                           "  ||   ",
                                           "    *  ",
                                           "  ||   ",
                                           "||     ",
                                       }));
    mSections.emplace_back(loadSection(mAssets.backgrounds,
                                       mWallTrigger.get(),
                                       mBonusTrigger.get(),
                                       {
                                           " *** *  * *** ",
                                           " * * ** *  *  ",
                                           " *** * **  *  ",
                                           " * * *  *  *  ",
                                           " * * *  *  *  ",
                                       }));
}

void WorldImpl::fillTriggers(ColumnArray<const Trigger*>& triggers) {
    int wallLane = -1;
    int maxLane = int(triggers.size() - 1);
    if (randomBool() == 0) {
        wallLane = randomRange(1, maxLane);
        triggers[wallLane] = mWallTrigger.get();
    }

    if (randomRange(4) == 0) {
        int bonusLane = wallLane;
        while (bonusLane == wallLane) {
            bonusLane = randomRange(1, maxLane);
        }
        triggers[bonusLane] = mBonusTrigger.get();
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
    const int roadPixelHeight = LANE_COUNT * TILE_SIZE;
    return (SCREEN_HEIGHT - roadPixelHeight) / 2 + lane * TILE_SIZE;
}

void WorldImpl::addGameObject(GameObject* gameObject) {
    mGameObjects.push_back(gameObject);
}

const Section* WorldImpl::getSection() const {
    return &randomChoice(mSections);
}
