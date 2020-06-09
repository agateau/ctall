#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <list>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

class World;
class Scroller;

struct BackgroundAssets {
    SDL2pp::Texture border;
    std::vector<SDL2pp::Texture> roads;
};

/**
 * Triggers something when the cell is about to become visible
 */
class Trigger {
public:
    virtual ~Trigger();
    virtual void exec(World& world, const SDL2pp::Point& position) const = 0;
};

struct Section {
    struct Column {
        const std::vector<const SDL2pp::Texture*> images;
        const std::vector<const Trigger*> triggers;
    };
    const std::vector<Column> columns;
};

using SectionList = std::list<const Section*>;

class Background {
public:
    class SectionProvider {
    public:
        virtual const Section* getSection() const = 0;
    };

    Background(World& world,
               const Scroller& scroller,
               const SectionProvider& sectionProvider);

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
