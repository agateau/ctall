#include "Background.h"

#include "Assets.h"
#include "GameScreen.h"
#include "Random.h"
#include "constants.h"

using namespace SDL2pp;

static constexpr int COLUMN_COUNT = SCREEN_WIDTH / LANE_WIDTH;
static constexpr int LANE_COUNT = MAX_LANE - MIN_LANE + 1;

Background::Background(GameScreen& gameScreen,
                       const Scroller& scroller,
                       std::vector<BackgroundAssets>& levelAssets)
        : mGameScreen(gameScreen), mScroller(scroller), mBackgroundAssets(levelAssets) {
    for (int idx = 0; idx < COLUMN_COUNT + 2; ++idx) {
        Column column(LANE_COUNT + 2); // +2 for borders
        fillColumn(column);
        mColumns.push_back(column);
    }
}

void Background::update() {
    int oldOffset = mOffset;
    mOffset = -int(mScroller.getPosition()) % LANE_WIDTH;
    if (mOffset <= oldOffset) {
        return;
    }
    Column column = *mColumns.begin();
    mColumns.pop_front();
    fillColumn(column);
    mColumns.push_back(column);
}

void Background::draw(SDL2pp::Renderer& renderer) {
    int x = mOffset;
    int startY = mGameScreen.yForLane(MIN_LANE - 1);
    for (auto& column : mColumns) {
        int y = startY;
        for (auto* texture : column) {
            renderer.Copy(*texture, NullOpt, {x, y});
            y += LANE_WIDTH;
        }
        x += LANE_WIDTH;
    }
}

void Background::setLevel(int level) {
    mLevel = level;
}

void Background::fillColumn(Background::Column& column) const {
    BackgroundAssets& assets = mBackgroundAssets.at(mLevel % mBackgroundAssets.size());
    auto it = column.begin();
    auto last = column.end() - 1;
    *it = &assets.border;
    ++it;
    for (; it != last; ++it) {
        *it = &Random::randomChoice(assets.roads);
    }
    *last = &assets.border;
}
