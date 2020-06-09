#include "Background.h"

#include "Assets.h"
#include "Random.h"
#include "Scroller.h"
#include "World.h"
#include "constants.h"

// std
#include <iostream>
#include <numeric>

using namespace SDL2pp;
using std::size_t;

static const size_t MIN_COLUMN_COUNT = int(ceil(double(SCREEN_WIDTH) / LANE_WIDTH)) + 2;

Background::Background(const World& world,
                       const Scroller& scroller,
                       const SectionProvider& sectionProvider)
        : mWorld(world), mScroller(scroller), mSectionProvider(sectionProvider) {
}

void Background::update() {
    if (mSections.empty()) {
        fillSectionList();
    }
    int oldOffset = mOffset;
    mOffset = -int(mScroller.getPosition()) % LANE_WIDTH;
    if (mOffset <= oldOffset) {
        return;
    }
    ++mColumnIndex;
    if (mColumnIndex == mSections.front()->columns.size()) {
        // We are done with the first section
        mSections.pop_front();
        mColumnIndex = 0;
    }
    fillSectionList();
}

/**
 * Fill mSections until it has enough content to fill the screen
 */
void Background::fillSectionList() {
    size_t totalColumns = std::accumulate(mSections.cbegin(),
                                          mSections.cend(),
                                          0,
                                          [](size_t value, const auto* section) {
                                              return value + section->columns.size();
                                          })
                          - mColumnIndex;
    while (totalColumns < MIN_COLUMN_COUNT) {
        const auto* section = mSectionProvider.getSection();
        mSections.push_back(section);
        totalColumns += section->columns.size();
    }
}

void Background::draw(SDL2pp::Renderer& renderer) {
    int startY = mWorld.yForLane(MIN_LANE - 1);
    auto sectionIt = mSections.cbegin();

    auto columnIt = (*sectionIt)->columns.cbegin();
    for (size_t i = 0; i < mColumnIndex; ++i, ++columnIt) {
    }

    for (int x = mOffset; x < SCREEN_WIDTH; x += LANE_WIDTH) {
        int y = startY;
        for (auto* texture : *columnIt) {
            renderer.Copy(*const_cast<SDL2pp::Texture*>(texture), NullOpt, {x, y});
            y += LANE_WIDTH;
        }
        ++columnIt;
        if (columnIt == (*sectionIt)->columns.cend()) {
            ++sectionIt;
            assert(sectionIt != mSections.cend());
            columnIt = (*sectionIt)->columns.cbegin();
        }
    }
}
