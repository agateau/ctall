#include "App.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "Screen.h"

// std
#include <chrono>
#include <thread>

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::duration;

inline std::chrono::time_point<std::chrono::steady_clock> now() {
    return std::chrono::steady_clock::now();
}

App::App(const std::string& title, int width, int height, int scale)
        : mWindow(title,
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  width * scale,
                  height * scale,
                  SDL_WINDOW_SHOWN)
        , mRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) {
    mRenderer.SetScale(scale, scale);
}

void App::setScreen(std::unique_ptr<Screen> screen) {
    mScreen = std::move(screen);
}

void App::run() {
    assert(mScreen.get());
    auto loopStep = [this](float delta) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            mScreen->onEvent(event);
            if (event.type == SDL_QUIT) {
                quit();
            } else if (event.type == SDL_KEYDOWN) {
                mScreen->onKeyPressed(event.key);
            } else if (event.type == SDL_KEYUP) {
                mScreen->onKeyReleased(event.key);
            }
        }
        mScreen->update(delta);
        mRenderer.Clear();
        mScreen->draw(mRenderer);
        mRenderer.Present();
    };

    auto previousTime = now();
    while (mRunning) {
        auto currentTime = now();
        auto delta = duration_cast<duration<float>>(currentTime - previousTime);
        if (delta.count() > 0) {
            loopStep(delta.count());
            previousTime = currentTime;
        }
    }
}

void App::quit() {
    mRunning = false;
}
