#ifndef TEXTDRAWER_H
#define TEXTDRAWER_H

#include <SDL2pp/SDL2pp.hh>

class TextDrawer {
public:
    enum Align { LEFT = 1, HCENTER = 2, RIGHT = 4, TOP = 8, VCENTER = 16, BOTTOM = 32 };

    TextDrawer(SDL2pp::Texture& image, const std::string& alphabet, const SDL2pp::Point& charSize);

    void draw(SDL2pp::Renderer& renderer,
              const std::string_view& text,
              const SDL2pp::Point& position,
              int align = TOP | LEFT);

    const SDL2pp::Point& charSize() const {
        return mCharSize;
    }

private:
    void drawChar(SDL2pp::Renderer& renderer, int x, int y, std::string::size_type idx);
    SDL2pp::Texture& mImage;
    const std::string mAlphabet;
    const SDL2pp::Point mCharSize;
    const int mColumnCount;
};

#endif // TEXTDRAWER_H
