#include "TextDrawer.h"

using namespace SDL2pp;

TextDrawer::TextDrawer(Texture& image, const std::string& alphabet, const Point& charSize)
        : mImage(image)
        , mAlphabet(alphabet)
        , mCharSize(charSize)
        , mColumnCount(image.GetWidth() / mCharSize.x) {
}

static std::pair<int, int> computeTextSize(const std::string_view& text) {
    int rows = 1;
    int widestColumn = 0;
    int column = 0;
    for (char ch : text) {
        if (ch == '\n') {
            rows += 1;
            widestColumn = std::max(column, widestColumn);
            column = 0;
        } else {
            column += 1;
        }
    }
    widestColumn = std::max(column, widestColumn);
    return {widestColumn, rows};
}

void TextDrawer::draw(Renderer& renderer,
                      const std::string_view& text,
                      const Point& position,
                      int align) {
    auto [columns, rows] = computeTextSize(text);
    int textWidth = columns * mCharSize.x;
    int textHeight = rows * mCharSize.y;

    int x = position.x;
    int y = position.y;

    if (align & HCENTER) {
        x -= textWidth / 2;
    } else if (align & RIGHT) {
        x -= textWidth;
    }
    if (align & VCENTER) {
        y -= textHeight / 2;
    } else if (align & BOTTOM) {
        y -= textHeight;
    }

    int startX = x;
    for (char ch : text) {
        if (ch == '\n') {
            y += mCharSize.y;
            x = startX;
        } else {
            if (ch != ' ') {
                auto idx = mAlphabet.find(ch);
                assert(idx != std::string::npos);
                drawChar(renderer, x, y, idx);
            }
            x += mCharSize.x;
        }
    }
}

void TextDrawer::drawChar(SDL2pp::Renderer& renderer, int x, int y, std::string::size_type idx) {
    int row = int(idx) / mColumnCount;
    int column = idx % mColumnCount;
    Rect srcRect{{column * mCharSize.x, row * mCharSize.y}, mCharSize};
    renderer.Copy(mImage, srcRect, {x, y});
}
