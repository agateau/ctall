#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2pp/SDL2pp.hh>

struct Assets {
private:
    std::string mBaseDir;

public:
    Assets(SDL2pp::Renderer& renderer);

    SDL2pp::Texture player;
    SDL2pp::Texture wall;

private:
    SDL2pp::Texture load(SDL2pp::Renderer& renderer, const std::string& name);
};

#endif // ASSETS_H
