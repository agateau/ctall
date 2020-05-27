#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "Game.h"
#include "constants.h"

using namespace SDL2pp;

static constexpr int FPS = 60;

inline std::chrono::time_point<std::chrono::steady_clock> now() {
    return std::chrono::steady_clock::now();
}

int main(int argc, char** argv) {
    bool useFramerateLimit = argc == 2 && std::string(argv[1]) == "--ufl";
    std::cout << "useFramerateLimit=" << useFramerateLimit << '\n';

    SDL sdl(SDL_INIT_VIDEO);

    Window window{"SDL2Test",
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  SCREEN_WIDTH,
                  SCREEN_HEIGHT,
                  SDL_WINDOW_SHOWN};

    Renderer renderer{window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};

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
