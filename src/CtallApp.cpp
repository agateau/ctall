#include "CtallApp.h"

#include "GameScreen.h"
#include "StartScreen.h"
#include "constants.h"

CtallApp::CtallApp() : App("Catch them all", SCREEN_WIDTH, SCREEN_HEIGHT), mAssets(renderer()) {
}

void CtallApp::showStartScreen() {
    setScreen(std::make_unique<StartScreen>(*this));
}

void CtallApp::showGameScreen() {
    setScreen(std::make_unique<GameScreen>(*this));
}
