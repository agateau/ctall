#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "CtallApp.h"
#include "GameScreen.h"

using namespace SDL2pp;

int main(int argc, char** argv) {
    bool useFramerateLimit = argc == 2 && std::string(argv[1]) == "--ufl";
    std::cout << "useFramerateLimit=" << useFramerateLimit << '\n';

    SDL sdl(SDL_INIT_VIDEO);

    CtallApp app;
    app.showStartScreen();
    app.run();

    return EXIT_SUCCESS;
}
