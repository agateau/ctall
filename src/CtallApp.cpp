#include "CtallApp.h"

#include "GameScreen.h"
#include "StartScreen.h"
#include "constants.h"

static constexpr int SCREEN_WIDTH = 360;
static constexpr int SCREEN_HEIGHT = 240;
static constexpr int SCREEN_SCALE = 4;

CtallApp::CtallApp()
        : App("Catch them all", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE), mAssets(renderer()) {
}

void CtallApp::showStartScreen() {
    setScreen(std::make_unique<StartScreen>(*this));
}

void CtallApp::showGameScreen() {
    setScreen(std::make_unique<GameScreen>(*this));
}

void CtallApp::setSkaterIdx(int value) {
    mPlayerIdx = value;
}
