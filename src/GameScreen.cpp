#include "GameScreen.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "Assets.h"
#include "CtallApp.h"
#include "Scroller.h"
#include "Wall.h"

// std
#include <algorithm>

static int randint(int min, int max) {
    int random = std::rand();
    return min + random % (max - min + 1);
}

static constexpr int SCORE_ROUND = 100;
static constexpr int SCORE_PER_CAPTURE = 1000;

using namespace SDL2pp;

GameScreen::GameScreen(CtallApp& app)
        : mApp(app)
        , mAssets(app.assets())
        , mPlayer(*this, mAssets.player, mInput)
        , mScroller(*this)
        , mWallPool([this]() { return new Wall(*this, mWallPool, mScroller, mAssets.wall); })
        , mBonusPool(
              [this]() { return new Bonus(*this, mBonusPool, mScroller, mAssets.bonuses); }) {
    mGameObjects.push_back(&mPlayer);
}

GameScreen::~GameScreen() {
}

void GameScreen::spawnThings() {
    int wallLane = MIN_LANE - 1;
    if (randint(0, 1) == 0) {
        auto wall = mWallPool.get();
        wallLane = randint(MIN_LANE, MAX_LANE);
        wall->setup(wallLane);
        mGameObjects.push_back(wall);
    }

    if (randint(0, 3) == 0) {
        auto bonus = mBonusPool.get();
        int bonusLane = wallLane;
        while (bonusLane == wallLane) {
            bonusLane = randint(MIN_LANE, MAX_LANE);
        }
        bonus->setup(bonusLane);
        mGameObjects.push_back(bonus);
    }
}

void GameScreen::update(float delta) {
    if (mState != State::Running) {
        return;
    }
    mScroller.update(delta);
    // Do not use an iterator here since it might become invalid if mGameObjects is resized after
    // an addition
    for (int idx = int(mGameObjects.size() - 1); idx >= 0; --idx) {
        assert(mGameObjects.at(idx)->isActive());
        mGameObjects.at(idx)->update(delta);
    }
    mGameObjects.erase(std::remove_if(mGameObjects.begin(),
                                      mGameObjects.end(),
                                      [](GameObject* object) { return !object->isActive(); }),
                       mGameObjects.end());
}

void GameScreen::draw(Renderer& renderer) {
    for (auto* object : mGameObjects) {
        object->draw(renderer);
    }
    drawHud(renderer);
    if (mState == State::GameOver) {
        drawGameOverOverlay(renderer);
    }
}

void GameScreen::drawHud(Renderer& renderer) {
    static char scoreText[40];
    std::snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score());
    mAssets.textDrawer.draw(renderer, scoreText, {0, 0});
}

int GameScreen::score() const {
    int distance = int(mScroller.getPosition() / SCORE_ROUND) * SCORE_ROUND;
    return distance + mPlayer.capturedCount() * SCORE_PER_CAPTURE;
}

void GameScreen::drawGameOverOverlay(Renderer& renderer) {
    mAssets.textDrawer.draw(renderer,
                            "GAME OVER\n\n",
                            {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
                            TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mAssets.textDrawer.draw(renderer,
                            "PRESS SPACE TO RESTART",
                            {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
                            TextDrawer::HCENTER | TextDrawer::TOP);
}

void GameScreen::onKeyPressed(const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_UP) {
        mInput.up = true;
    } else if (event.keysym.sym == SDLK_DOWN) {
        mInput.down = true;
    }
}

void GameScreen::onKeyReleased(const SDL_KeyboardEvent& event) {
    if (mState == State::GameOver) {
        if (event.keysym.sym == SDLK_SPACE) {
            mApp.showGameScreen();
        }
        return;
    }
    if (event.keysym.sym == SDLK_UP) {
        mInput.up = false;
    } else if (event.keysym.sym == SDLK_DOWN) {
        mInput.down = false;
    }
}

int GameScreen::yForLane(int lane) const {
    return SCREEN_HEIGHT / 2 + lane * LANE_WIDTH;
}

const std::vector<GameObject*>& GameScreen::activeGameObjects() const {
    return mGameObjects;
}

void GameScreen::switchToGameOverState() {
    mState = State::GameOver;
}
