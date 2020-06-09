#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "CtallApp.h"

using namespace SDL2pp;

int main(int /*argc*/, char** /*argv*/) {
    SDL sdl(SDL_INIT_VIDEO);

    CtallApp app;
    app.showStartScreen();
    app.run();

    return EXIT_SUCCESS;
}
