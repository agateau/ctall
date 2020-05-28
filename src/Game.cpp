#include "Game.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>
#include <algorithm>

#include "Scroller.h"
#include "Wall.h"

static int randint(int min, int max) {
    int random = std::rand();
    return min + random % (max - min + 1);
}

using namespace SDL2pp;

Game::Game(Renderer& renderer)
        : mAssets(renderer)
        , mPlayer(*this, mAssets.player, mInput)
        , mScroller(*this)
        , mWallPool([this]() { return new Wall(*this, mWallPool, mScroller, mAssets.wall); })
        , mBonusPool(
              [this]() { return new Bonus(*this, mBonusPool, mScroller, mAssets.bonuses); }) {
    mGameObjects.push_back(&mPlayer);
}

Game::~Game() {
}

void Game::spawnThings() {
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

void Game::update(float delta) {
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

void Game::draw(Renderer& renderer) {
    renderer.Clear();
    for (auto* object : mGameObjects) {
        object->draw(renderer);
    }
    if (mState == State::GameOver) {
        drawGameOverOverlay(renderer);
    }
    renderer.Present();
}

void Game::drawGameOverOverlay(SDL2pp::Renderer& renderer) {
    mAssets.textDrawer.draw(renderer,
                            "GAME OVER",
                            {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
                            TextDrawer::HCENTER | TextDrawer::VCENTER);
}

void Game::onKeyPressed(const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_UP) {
        mInput.up = true;
    } else if (event.keysym.sym == SDLK_DOWN) {
        mInput.down = true;
    }
}

void Game::onKeyReleased(const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_UP) {
        mInput.up = false;
    } else if (event.keysym.sym == SDLK_DOWN) {
        mInput.down = false;
    }
}

int Game::yForLane(int lane) const {
    return SCREEN_HEIGHT / 2 + lane * LANE_WIDTH;
}

const std::vector<GameObject*>& Game::activeGameObjects() const {
    return mGameObjects;
}

void Game::switchToGameOverState() {
    mState = State::GameOver;
}
