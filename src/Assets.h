#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2pp/SDL2pp.hh>

#include "TextDrawer.h"

struct Assets {
private:
    std::string mBaseDir;
    SDL2pp::Texture textTexture;

public:
    Assets(SDL2pp::Renderer& renderer);

    SDL2pp::Texture player;
    SDL2pp::Texture wall;
    std::vector<SDL2pp::Texture> bonuses;
    TextDrawer textDrawer;

private:
    SDL2pp::Texture load(SDL2pp::Renderer& renderer, const std::string& name);
    std::vector<SDL2pp::Texture> loadAll(SDL2pp::Renderer& renderer, const std::string& dirName);
};

#endif // ASSETS_H
