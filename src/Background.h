#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Section.h"

// std
#include <list>
#include <vector>

class World;
class Scroller;

using SectionList = std::list<const Section*>;

class Background {
public:
    class SectionProvider {
    public:
        virtual const Section* getSection() const = 0;
    };

    Background(World& world, const Scroller& scroller, const SectionProvider& sectionProvider);

    void update();

    void draw(SDL2pp::Renderer& renderer);

private:
    void fillSectionList();

    World& mWorld;
    const Scroller& mScroller;
    const SectionProvider& mSectionProvider;
    SectionList mSections;
    int mOffset = 0;
    std::size_t mColumnIndex = 0;
};

#endif // BACKGROUND_H
