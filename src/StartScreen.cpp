#include "StartScreen.h"

#include "Assets.h"
#include "CtallApp.h"
#include "constants.h"

#include <SDL2/SDL.h>

StartScreen::StartScreen(CtallApp& app) : mApp(app), mAssets(mApp.assets()) {
}

void StartScreen::onKeyReleased(const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_SPACE) {
        mApp.showGameScreen();
    }
}

void StartScreen::draw(SDL2pp::Renderer& renderer) {
    mAssets.textDrawer.draw(renderer,
                            "PRESS SPACE TO START",
                            {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
                            TextDrawer::HCENTER | TextDrawer::VCENTER);
}
