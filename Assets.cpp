#include "Assets.h"

#include <SDL2pp/SDL2pp.hh>

#include <cassert>

using namespace SDL2pp;

Assets::Assets(Renderer& renderer)
    : mBaseDir("images")
    , player(load(renderer, "player"))
    , wall(load(renderer, "wall"))
{
}

Texture Assets::load(Renderer& renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    return Texture(renderer, path);
}
