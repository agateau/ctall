#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <list>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

class GameScreen;
class BackgroundAssets;
class Scroller;

struct BackgroundAssets {
    SDL2pp::Texture border;
    std::vector<SDL2pp::Texture> roads;
};

class Background {
    using Column = std::vector<SDL2pp::Texture*>;
public:
    Background(GameScreen& gameScreen,
               const Scroller& scroller,
               std::vector<BackgroundAssets>& backgroundAssets);

    void update();

    void draw(SDL2pp::Renderer& renderer);

    void setLevel(int level);

private:
    void fillColumn(Column& column) const;
    GameScreen& mGameScreen;
    const Scroller& mScroller;
    std::vector<BackgroundAssets>& mBackgroundAssets;
    int mLevel = 0;
    std::list<Column> mColumns;
    int mOffset = 0;
};

#endif // BACKGROUND_H
