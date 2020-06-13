#include "StartScreen.h"

#include "Assets.h"
#include "CtallApp.h"

StartScreen::StartScreen(CtallApp& app)
        : mApp(app), mAssets(mApp.assets()), mMenu(mAssets.textDrawer) {
    mMenu.addItem("START", [this]() { mApp.showGameScreen(); });
    mMenu.addItem(generateSkaterItem(), [this]() { selectNextSkater(); });
    mMenu.addItem("QUIT", [this]() { mApp.quit(); });
}

void StartScreen::onEvent(const SDL_Event& event) {
    mMenu.onEvent(event);
}

void StartScreen::draw(SDL2pp::Renderer& renderer) {
    auto center = mApp.screenSize() / 2;

    mAssets.textDrawer.draw(
        renderer, "CATCH THEM ALL!\n\n", center, TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mMenu.draw(renderer, center);
}

std::string StartScreen::generateSkaterItem() const {
    return "SKATER: " + mAssets.skaterNames.at(mApp.skaterIdx());
}

void StartScreen::selectNextSkater() {
    mApp.setSkaterIdx((mApp.skaterIdx() + 1) % AVAILABLE_SKATER_COUNT);
    mMenu.setItemText(1, generateSkaterItem());
}
