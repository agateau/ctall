#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Menu.h"
#include "Screen.h"

class Assets;
class CtallApp;

class StartScreen : public Screen {
public:
    StartScreen(CtallApp& app);
    void onEvent(const SDL_Event& event) override;
    void draw(SDL2pp::Renderer& renderer) override;

private:
    std::string generateSkaterItem() const;
    void selectNextSkater();
    CtallApp& mApp;
    Assets& mAssets;
    Menu mMenu;
};

#endif // STARTSCREEN_H
