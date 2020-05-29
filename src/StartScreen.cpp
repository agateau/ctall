#include "StartScreen.h"

#include "Assets.h"
#include "CtallApp.h"
#include "constants.h"

StartScreen::StartScreen(CtallApp& app)
        : mApp(app), mAssets(mApp.assets()), mMenu(mAssets.textDrawer) {
    mMenu.addItem("START", [this]() { mApp.showGameScreen(); });
    mMenu.addItem("QUIT", [this]() { mApp.quit(); });
}

void StartScreen::onEvent(const SDL_Event& event) {
    mMenu.onEvent(event);
}

void StartScreen::draw(SDL2pp::Renderer& renderer) {
    mAssets.textDrawer.draw(renderer,
                            "CATCH THEM ALL!\n\n",
                            {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
                            TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mMenu.draw(renderer, {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
}
