#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Screen.h"

class Assets;
class CtallApp;

class StartScreen : public Screen {
public:
    StartScreen(CtallApp& app);
    void onKeyReleased(const SDL_KeyboardEvent& event) override;
    void draw(SDL2pp::Renderer& renderer) override;

private:
    CtallApp& mApp;
    Assets& mAssets;
};

#endif // STARTSCREEN_H
