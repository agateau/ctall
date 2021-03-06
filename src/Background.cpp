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
                       SectionProvider& sectionProvider,
                       const TriggerMap& triggers,
                       const SDL2pp::Point& screenSize)
        : mMinColumnCount(size_t(ceil(double(screenSize.x) / TILE_SIZE)) + 2)
        , mScreenSize(screenSize)
        , mWorld(world)
        , mScroller(scroller)
        , mSectionProvider(sectionProvider)
        , mTriggers(triggers) {
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
    columnIt += mColumnIndex + mMinColumnCount - 1;
    Point pos(int((mMinColumnCount - 1) * TILE_SIZE) + mOffset,
              mWorld.yForLane(-Section::BORDER_HEIGHT));
    for (auto triggerId : columnIt->triggers) {
        if (triggerId != TriggerId::None) {
            mTriggers.at(triggerId)->exec(mWorld, pos);
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
    while (totalColumns < mMinColumnCount) {
        const auto* section = mSectionProvider.getSection();
        mSections.push_back(section);
        totalColumns += section->columns.size();
    }
}

void Background::draw(SDL2pp::Renderer& renderer) {
    int startY = mWorld.yForLane(-Section::BORDER_HEIGHT);
    ColumnIterator columnIt(mSections);
    columnIt += mColumnIndex;

    SDL2pp::Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
    for (int x = mOffset; x < mScreenSize.x; x += TILE_SIZE, ++columnIt) {
        for (const auto& layer : columnIt->layers) {
            int y = startY;
            for (auto* tile : layer) {
                if (tile) {
                    rect.x = tile->pos.x;
                    rect.y = tile->pos.y;
                    renderer.Copy(const_cast<SDL2pp::Texture&>(tile->image), rect, {x, y});
                }
                y += TILE_SIZE;
            }
        }
    }
}
