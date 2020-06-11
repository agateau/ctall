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

static const size_t MIN_COLUMN_COUNT = int(ceil(double(SCREEN_WIDTH) / TILE_SIZE)) + 2;

/**
 * An iterator to iterate over columns stored in a list of sections.
 * Not STL compliant: it asserts if it goes past the end of the columns, among other differences.
 */
class ColumnIterator {
public:
    ColumnIterator(const SectionList& sections)
            : mSections(sections), mSectionIt(sections.cbegin()) {
    }

    const Section::Column& operator*() const {
        return (*mSectionIt)->columns.at(mColumnIndex);
    }

    const Section::Column* operator->() const {
        return &(*mSectionIt)->columns.at(mColumnIndex);
    }

    void operator++() {
        ++mColumnIndex;
        if (mColumnIndex == (*mSectionIt)->columns.size()) {
            ++mSectionIt;
            assert(mSectionIt != mSections.cend());
            mColumnIndex = 0;
        }
    }

    void operator+=(size_t offset) {
        mColumnIndex += offset;
        while (true) {
            auto sectionSize = (*mSectionIt)->columns.size();
            if (mColumnIndex < sectionSize) {
                return;
            } else {
                ++mSectionIt;
                assert(mSectionIt != mSections.cend());
                mColumnIndex -= sectionSize;
            }
        }
    }

private:
    const std::list<const Section*>& mSections;
    SectionList::const_iterator mSectionIt;
    size_t mColumnIndex = 0;
};

Background::Background(World& world,
                       const Scroller& scroller,
                       const SectionProvider& sectionProvider)
        : mWorld(world), mScroller(scroller), mSectionProvider(sectionProvider) {
}

void Background::update() {
    if (mSections.empty()) {
        fillSectionList();
    }
    int oldOffset = mOffset;
    mOffset = -int(mScroller.getPosition()) % TILE_SIZE;
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

    ColumnIterator columnIt(mSections);
    columnIt += mColumnIndex + MIN_COLUMN_COUNT - 1;
    Point pos(int((MIN_COLUMN_COUNT - 1) * TILE_SIZE), mWorld.yForLane(-1));
    for (auto* trigger : columnIt->triggers) {
        if (trigger) {
            trigger->exec(mWorld, pos);
        }
        pos.y += TILE_SIZE;
    }
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
    int startY = mWorld.yForLane(-1);
    ColumnIterator columnIt(mSections);
    columnIt += mColumnIndex;

    SDL2pp::Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (int x = mOffset; x < SCREEN_WIDTH; x += TILE_SIZE, ++columnIt) {
        int y = startY;
        for (auto* tile : columnIt->tiles) {
            rect.x = tile->pos.x;
            rect.y = tile->pos.y;
            renderer.Copy(const_cast<SDL2pp::Texture&>(tile->image), rect, {x, y});
            y += TILE_SIZE;
        }
    }
}
