#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <list>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

class World;
class BackgroundAssets;
class Scroller;

struct BackgroundAssets {
    SDL2pp::Texture border;
    std::vector<SDL2pp::Texture> roads;
};

struct Section {
    using Column = std::vector<const SDL2pp::Texture*>;
    const std::vector<Column> columns;
};

class Background {
public:
    class SectionProvider {
    public:
        virtual const Section* getSection() const = 0;
    };

    Background(const World& world,
               const Scroller& scroller,
               const SectionProvider& sectionProvider);

    void update();

    void draw(SDL2pp::Renderer& renderer);

private:
    void fillSectionList();

    const World& mWorld;
    const Scroller& mScroller;
    const SectionProvider& mSectionProvider;
    std::list<const Section*> mSections;
    int mOffset = 0;
    std::size_t mColumnIndex = 0;
};

#endif // BACKGROUND_H
