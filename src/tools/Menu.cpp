#include "Menu.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "TextDrawer.h"

Menu::Menu(TextDrawer& textDrawer) : mTextDrawer(textDrawer) {
}

void Menu::onEvent(const SDL_Event& event) {
    if (event.type != SDL_KEYUP) {
        return;
    }
    auto sym = event.key.keysym.sym;
    if (sym == SDLK_DOWN) {
        mCurrent = (mCurrent + 1) % mItems.size();
    } else if (sym == SDLK_UP) {
        if (mCurrent > 0) {
            mCurrent--;
        } else {
            mCurrent = mItems.size() - 1;
        }
    } else if (sym == SDLK_SPACE || sym == SDLK_RETURN) {
        mItems.at(mCurrent).callback();
    }
}

void Menu::addItem(const std::string& text, Menu::Callback callback) {
    mItems.push_back({text, callback});
}

void Menu::draw(SDL2pp::Renderer& renderer, const SDL2pp::Point& pos) {
    int y = pos.y;
    int centerX = pos.x;
    for (std::size_t idx = 0; idx < mItems.size(); ++idx) {
        std::string text = "  " + mItems.at(idx).text + "  ";
        if (idx == mCurrent) {
            text[0] = '>';
            text[text.size() - 1] = '<';
        }
        mTextDrawer.draw(renderer, text, {centerX, y}, TextDrawer::HCENTER | TextDrawer::TOP);
        y += mTextDrawer.charSize().y * 2;
    }
}
