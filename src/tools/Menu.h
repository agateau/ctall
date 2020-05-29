#ifndef MENU_H
#define MENU_H

// std
#include <algorithm>
#include <vector>

class TextDrawer;

union SDL_Event;

namespace SDL2pp {
class Renderer;
class Point;
} // namespace SDL2pp

class Menu {
public:
    using Callback = std::function<void()>;

    Menu(TextDrawer& textDrawer);

    void onEvent(const SDL_Event& event);

    void addItem(const std::string& text, Callback callback);

    void draw(SDL2pp::Renderer& renderer, const SDL2pp::Point& pos);

private:
    struct MenuItem {
        std::string text;
        Callback callback;
    };
    TextDrawer& mTextDrawer;
    std::vector<MenuItem> mItems;
    std::size_t mCurrent = 0;
};

#endif // MENU_H
