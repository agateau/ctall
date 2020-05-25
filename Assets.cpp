#include "Assets.h"

#include <cassert>

Assets::Assets(SDL_Renderer* renderer) {
    mBaseDir = "images";
    player.reset(load(renderer, "player"));
    wall.reset(load(renderer, "wall"));
}

SDL_Texture* Assets::load(SDL_Renderer* renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    auto texture = loadTexture(renderer, path);
    assert(texture);
    return texture;
}
