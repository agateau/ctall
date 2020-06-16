#include "App.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "Screen.h"

// std
#include <chrono>
#include <thread>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;

// A float-based second
using fsec = duration<float>;

static constexpr bool VSYNC = false;
static constexpr int FPS = 120;
static constexpr bool FULLSCREEN = false;

inline std::chrono::time_point<std::chrono::steady_clock> now() {
    return std::chrono::steady_clock::now();
}

App::App(const std::string& title, int width, int height, int scale)
        : mScale(scale)
        , mWindow(title,
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  width * scale,
                  height * scale,
                  SDL_WINDOW_SHOWN)
        , mRenderer(
              mWindow, -1, SDL_RENDERER_ACCELERATED | (VSYNC ? SDL_RENDERER_PRESENTVSYNC : 0)) {
    if (FULLSCREEN) {
        mWindow.SetFullscreen(SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    mRenderer.SetScale(scale, scale);
    mScreenSize = mWindow.GetSize() / mScale;
}

App::~App() = default;

void App::setScreen(std::unique_ptr<Screen> screen) {
    mScreen = std::move(screen);
}

void App::run() {
    assert(mScreen.get());

    if (VSYNC) {
        vsyncRun();
    } else {
        noVsyncRun();
    }
}

void App::vsyncRun() {
    auto previousTime = now();
    while (mRunning) {
        auto currentTime = now();
        auto delta = duration_cast<fsec>(currentTime - previousTime);
        if (delta.count() > 0) {
            loopStep(delta.count());
            previousTime = currentTime;
        }
    }
}

void App::noVsyncRun() {
    const auto delta = fsec(1.0f / FPS);

    auto nextTime = now();
    while (mRunning) {
        auto currentTime = now();
        if (currentTime > nextTime) {
            nextTime += duration_cast<nanoseconds>(delta);
            loopStep(delta.count());
        } else {
            auto sleepTime = nextTime - currentTime;
            std::this_thread::sleep_for(sleepTime);
        }
    }
}

void App::quit() {
    mRunning = false;
}

void App::loopStep(float delta) {
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
}
