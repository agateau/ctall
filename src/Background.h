#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Section.h"

// SDL2pp
#include <SDL2pp/SDL2pp.hh>

// std
#include <list>
#include <vector>

class World;
class Scroller;

using SectionList = std::list<const Section*>;

using TriggerMap = std::unordered_map<TriggerId, const Trigger*>;

class Background {
public:
    class SectionProvider {
    public:
        virtual const Section* getSection() = 0;
    };

    Background(World& world,
               const Scroller& scroller,
               SectionProvider& sectionProvider,
               const TriggerMap& triggers,
               const SDL2pp::Point& screenSize);

    void update();

    void draw(SDL2pp::Renderer& renderer);

private:
    void fillSectionList();
    const std::size_t mMinColumnCount;
    const SDL2pp::Point& mScreenSize;

    World& mWorld;
    const Scroller& mScroller;
    SectionProvider& mSectionProvider;
    const TriggerMap& mTriggers;
    SectionList mSections;
    int mOffset = 0;
    std::size_t mColumnIndex = 0;
};

#endif // BACKGROUND_H
