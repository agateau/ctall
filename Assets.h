#ifndef ASSETS_H
#define ASSETS_H

#include "MySDLUtils.h"

struct Assets {
    Assets(SDL_Renderer* renderer);

    Texture player;
    Texture wall;

private:
    std::string mBaseDir;

    SDL_Texture* load(SDL_Renderer* renderer, const std::string& name);
};

#endif // ASSETS_H
