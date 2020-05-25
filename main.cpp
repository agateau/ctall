#include "Assets.h"
#include "constants.h"
#include "GameObject.h"
#include "ScrollComponent.h"
#include "Scroller.h"
#include "Pool.h"
#include "Wall.h"

#include "MySDLUtils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

static int randint(int min, int max) {
    int random = std::rand();
    return min + random % (max - min + 1);
}

static int FPS = 60;

static float PX_PER_SEC = 90;

struct Input {
    bool up = false;
    bool down = false;
};

class Player : public GameObject {
public:
    Player(SDL_Texture* texture, const Input& input)
        : mTexture(texture), mInput(input), mPos{12, SCREEN_HEIGHT / 2} {
    }

    void update(float delta) override {
        if (mInput.up) {
            mPos.y -= PX_PER_SEC * delta;
        } else if (mInput.down) {
            mPos.y += PX_PER_SEC * delta;
        }
    }

    void draw(const Renderer& renderer) override {
        renderer.renderTexture(mTexture, mPos.x, mPos.y);
    }

private:
    SDL_Texture* mTexture;
    const Input& mInput;
    SDL_Point mPos;
};

class Game : public Scroller::Listener {
public:
    Game(SDL_Renderer* renderer)
        : mAssets(renderer)
        , mPlayer(mAssets.player.get(), mInput)
        , mScroller(*this)
        , mWallPool([this]() { return new Wall(mWallPool, mScroller, mAssets.wall.get()); })
    {
    }

    void spawnThings() override {
        if (randint(0, 1) == 0) {
            auto wall = mWallPool.get();
            int row = randint(MIN_ROW, MAX_ROW);
            wall->setup(row);
        }
    }

    void update(float delta) {
        mPlayer.update(delta);
        mScroller.update(delta);
        for (auto* item : mWallPool.getActiveItems()) {
            item->update(delta);
        }
    }

    void draw(Renderer& renderer) {
        SDL_RenderClear(renderer.get());
        mPlayer.draw(renderer);
        for (auto* item : mWallPool.getActiveItems()) {
            item->draw(renderer);
        }
        SDL_RenderPresent(renderer.get());
    }

    void onKeyDown(const SDL_KeyboardEvent& event) {
        if (event.keysym.sym == SDLK_UP) {
            mInput.up = true;
        } else if (event.keysym.sym == SDLK_DOWN) {
            mInput.down = true;
        }
    }

    void onKeyUp(const SDL_KeyboardEvent& event) {
        if (event.keysym.sym == SDLK_UP) {
            mInput.up = false;
        } else if (event.keysym.sym == SDLK_DOWN) {
            mInput.down = false;
        }
    }

private:
    Assets mAssets;
    Input mInput;
    Player mPlayer;
    Pool<Wall> mWallPool;
    Scroller mScroller;
};

class App {
public:
    App() : mOk(SDL_Init(SDL_INIT_VIDEO) == 0) {
        if (!mOk) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        }
    }

    ~App() {
        if (mOk) {
            SDL_Quit();
        }
    }

    bool ok() const {
        return mOk;
    }

private:
    bool mOk;
};


inline std::chrono::time_point<std::chrono::steady_clock> now() {
    return std::chrono::steady_clock::now();
}


int main(int argc, char** argv) {
    bool useFramerateLimit = argc == 2 && std::string(argv[1]) == "--ufl";
    std::cout << "useFramerateLimit=" << useFramerateLimit << '\n';

    App app;
    if (!app.ok()) {
        return EXIT_FAILURE;
    }

    Window window{SDL_CreateWindow(
            "SDL2Test",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN)};
    if (!window) {
        return EXIT_FAILURE;
    }

    Renderer renderer{SDL_CreateRenderer(window.get(), -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)};
    if (!renderer) {
        std::cerr << "Failed to create renderer " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    Game game(renderer.get());

    auto loopStep = [&renderer, &game](float delta) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
                game.onKeyDown(event.key);
            } else if (event.type == SDL_KEYUP) {
                game.onKeyUp(event.key);
            }
        }
        game.update(delta);
        game.draw(renderer);
        return true;
    };

    float delta = 1.0 / FPS;
    if (useFramerateLimit) {
        while (true) {
            if (!loopStep(delta)) {
                break;
            }
        }
    } else {
        auto nextTime = now();
        while (true) {
            auto currentTime = now();
            if (currentTime >= nextTime) {
                nextTime += std::chrono::microseconds(int(1000 * delta));
                if (!loopStep(delta)) {
                    break;
                }
            } else {
                auto duration = nextTime - currentTime;
                if (duration > std::chrono::microseconds(0)) {
                    std::this_thread::sleep_for(duration);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
