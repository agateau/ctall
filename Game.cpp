#include "Game.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

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
        , mWallPool([this]() { return new Wall(*this, mWallPool, mScroller, mAssets.wall); }) {
}

Game::~Game() {
}

void Game::spawnThings() {
    if (randint(0, 1) == 0) {
        auto wall = mWallPool.get();
        int lane = randint(MIN_LANE, MAX_LANE);
        wall->setup(lane);
    }
}

void Game::update(float delta) {
    mPlayer.update(delta);
    mScroller.update(delta);
    for (auto* item : mWallPool.getActiveItems()) {
        item->update(delta);
    }
}

void Game::draw(Renderer& renderer) {
    renderer.Clear();
    mPlayer.draw(renderer);
    for (auto* item : mWallPool.getActiveItems()) {
        item->draw(renderer);
    }
    renderer.Present();
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
