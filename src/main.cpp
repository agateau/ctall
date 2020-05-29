#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "App.h"
#include "Game.h"
#include "constants.h"

using namespace SDL2pp;

int main(int argc, char** argv) {
    bool useFramerateLimit = argc == 2 && std::string(argv[1]) == "--ufl";
    std::cout << "useFramerateLimit=" << useFramerateLimit << '\n';

    SDL sdl(SDL_INIT_VIDEO);

    App app("Catch them all", SCREEN_WIDTH, SCREEN_HEIGHT);

    app.run(std::make_unique<Game>(app.renderer()));

    return EXIT_SUCCESS;
}
