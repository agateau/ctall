#include "GameScreen.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "Assets.h"
#include "CtallApp.h"
#include "WorldImpl.h"

using namespace SDL2pp;


GameScreen::GameScreen(CtallApp& app)
        : mApp(app)
        , mAssets(app.assets())
        , mWorld(app.assets(), mInput, mApp.screenSize(), mApp.skaterIdx())
        , mGameOverMenu(mAssets.textDrawer)
        , mPauseMenu(mAssets.textDrawer) {
    mGameOverMenu.addItem("RESTART", [&app] { app.showGameScreen(); });
    mGameOverMenu.addItem("BACK TO MENU", [&app] { app.showStartScreen(); });

    mPauseMenu.addItem("CONTINUE", [this] { mWorld.switchToRunningState(); });
    mPauseMenu.addItem("BACK TO MENU", [&app] { app.showStartScreen(); });
}

GameScreen::~GameScreen() {
}

void GameScreen::update(float delta) {
    if (mWorld.state() != WorldImpl::State::Running) {
        return;
    }
    mWorld.update(delta);
}

void GameScreen::draw(Renderer& renderer) {
    mWorld.getBackground().draw(renderer);

    for (auto* object : mWorld.gameObjects()) {
        object->draw(renderer);
    }
    drawHud(renderer);
    if (mWorld.state() == WorldImpl::State::GameOver) {
        drawGameOverOverlay(renderer);
    } else if (mWorld.state() == WorldImpl::State::Paused) {
        drawPauseOverlay(renderer);
    }
}

void GameScreen::onEvent(const SDL_Event& event) {
    if (mWorld.state() == WorldImpl::State::GameOver) {
        mGameOverMenu.onEvent(event);
    } else if (mWorld.state() == WorldImpl::State::Paused) {
        mPauseMenu.onEvent(event);
    }
}

void GameScreen::drawHud(Renderer& renderer) {
    static char scoreText[40];
    std::snprintf(scoreText, sizeof(scoreText), "SCORE: %d", mWorld.score());
    mAssets.textDrawer.draw(renderer, scoreText, {0, 0});
}

void GameScreen::drawGameOverOverlay(Renderer& renderer) {
    Point center = mApp.screenSize() / 2;
    mAssets.textDrawer.draw(
        renderer, "GAME OVER\n\n", center, TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mGameOverMenu.draw(renderer, center);
}

void GameScreen::drawPauseOverlay(Renderer& renderer) {
    Point center = mApp.screenSize() / 2;
    mAssets.textDrawer.draw(
        renderer, "PAUSED\n\n", center, TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mPauseMenu.draw(renderer, center);
}

void GameScreen::onKeyPressed(const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_UP) {
        mInput.up = true;
    } else if (event.keysym.sym == SDLK_DOWN) {
        mInput.down = true;
    }
}

void GameScreen::onKeyReleased(const SDL_KeyboardEvent& event) {
    auto sym = event.keysym.sym;
    if (mWorld.state() == WorldImpl::State::GameOver) {
        if (sym == SDLK_SPACE) {
            mApp.showGameScreen();
        }
        return;
    }
    if (sym == SDLK_UP) {
        mInput.up = false;
    } else if (sym == SDLK_DOWN) {
        mInput.down = false;
    } else if (sym == SDLK_ESCAPE) {
        mWorld.switchToPauseState();
    }
}
