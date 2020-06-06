#include "App.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "Screen.h"

// std
#include <chrono>
#include <thread>

static constexpr int FPS = 60;

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

    float delta = 1.0 / FPS;
    auto nextTime = now();
    while (mRunning) {
        auto currentTime = now();
        if (currentTime >= nextTime) {
            nextTime += std::chrono::microseconds(int(1000 * delta));
            loopStep(delta);
        } else {
            auto duration = nextTime - currentTime;
            if (duration > std::chrono::microseconds(0)) {
                std::this_thread::sleep_for(duration);
            }
        }
    }
}

void App::quit() {
    mRunning = false;
}
