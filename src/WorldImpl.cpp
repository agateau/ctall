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

static Section generateSection(const vector<TileSet>& tileSets, size_t columnCount) {
    Section section;
    const auto& tileSet = randomChoice(tileSets);
    for (size_t columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
        ColumnArray<const Tile*> tiles;
        ColumnArray<const Trigger*> triggers;

        auto it = tiles.begin();
        auto last = tiles.end() - 1;
        *it = &tileSet.tile(TileSet::BORDER);
        ++it;
        for (; it != last; ++it) {
            auto id = Random::randomChoice<TileSet::TileId>({TileSet::ROAD0, TileSet::ROAD1});
            *it = &tileSet.tile(id);
        }
        *last = &tileSet.tile(TileSet::BORDER);
        std::fill(triggers.begin(), triggers.end(), nullptr);
        section.columns.emplace_back(Section::Column{tiles, triggers});
    }
    return section;
}

static Section loadSection(const vector<TileSet>& tileSets,
                           Trigger* wallTrigger,
                           Trigger* bonusTrigger,
                           const vector<string>& lines) {
    assert(lines.size() == LANE_COUNT);
    auto columnCount = lines.front().size();

    Section section = generateSection(tileSets, columnCount);

    for (size_t columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
        auto& triggers = section.columns.at(columnIdx).triggers;
        for (size_t row = 0; row < lines.size(); ++row) {
            auto ch = lines.at(row).at(columnIdx);
            if (ch == '|') {
                triggers.at(row + 1) = wallTrigger;
            } else if (ch == '*') {
                triggers.at(row + 1) = bonusTrigger;
            }
        }
    }

    return section;
}

/**
 * At one point sections will be created from assets
 */
void WorldImpl::createSections() {
    for (int i = 0; i < SECTION_COUNT; ++i) {
        int length = randomRange(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH);
        Section section = generateSection(mAssets.tileSets, length);

        for (int columnIdx = 0; columnIdx < length; ++columnIdx) {
            if (columnIdx % SPAWN_SPACING == 0) {
                fillTriggers(section.columns.at(columnIdx).triggers);
            }
        }
        mSections.emplace_back(section);
    }
    mSections.emplace_back(loadSection(mAssets.tileSets,
                                       mWallTrigger.get(),
                                       mBonusTrigger.get(),
                                       {
                                           "||     ",
                                           "  ||   ",
                                           "    *  ",
                                           "  ||   ",
                                           "||     ",
                                       }));
    mSections.emplace_back(loadSection(mAssets.tileSets,
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
