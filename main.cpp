#include "Assets.h"
#include "constants.h"
#include "GameObject.h"
#include "ScrollComponent.h"
#include "Scroller.h"
#include "Pool.h"
#include "Wall.h"

#include <SDL2pp/SDL2pp.hh>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace SDL2pp;

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
    Player(Texture& texture, const Input& input)
        : mTexture(texture), mInput(input), mPos{12, SCREEN_HEIGHT / 2} {
    }

    void update(float delta) override {
        if (mInput.up) {
            mPos.y -= PX_PER_SEC * delta;
        }
        if (mInput.down) {
            mPos.y += PX_PER_SEC * delta;
        }
    }

    void draw(Renderer& renderer) override {
        renderer.Copy(mTexture, NullOpt, mPos);
    }

private:
    Texture& mTexture;
    const Input& mInput;
    Point mPos;
};

class Game : public Scroller::Listener {
public:
    Game(Renderer& renderer)
        : mAssets(renderer)
        , mPlayer(mAssets.player, mInput)
        , mScroller(*this)
        , mWallPool([this]() { return new Wall(mWallPool, mScroller, mAssets.wall); })
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
        renderer.Clear();
        mPlayer.draw(renderer);
        for (auto* item : mWallPool.getActiveItems()) {
            item->draw(renderer);
        }
        renderer.Present();
    }

    void onKeyPressed(const SDL_KeyboardEvent& event) {
        if (event.keysym.sym == SDLK_UP) {
            mInput.up = true;
        } else if (event.keysym.sym == SDLK_DOWN) {
            mInput.down = true;
        }
    }

    void onKeyReleased(const SDL_KeyboardEvent& event) {
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
    Scroller mScroller;
    Pool<Wall> mWallPool;
};

inline std::chrono::time_point<std::chrono::steady_clock> now() {
    return std::chrono::steady_clock::now();
}

int main(int argc, char** argv) {
    bool useFramerateLimit = argc == 2 && std::string(argv[1]) == "--ufl";
    std::cout << "useFramerateLimit=" << useFramerateLimit << '\n';

    SDL sdl(SDL_INIT_VIDEO);

    Window window{
            "SDL2Test",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN};

    Renderer renderer{window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

    Game game(renderer);

    auto loopStep = [&renderer, &game](float delta) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return false;
                }
                game.onKeyPressed(event.key);
            } else if (event.type == SDL_KEYUP) {
                game.onKeyReleased(event.key);
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
