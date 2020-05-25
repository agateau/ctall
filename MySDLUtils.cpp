#include "MySDLUtils.h"

#include <SDL2/SDL_image.h>

#include <iostream>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Failed to create texture from " << path << "\n";
    }
    return texture;
}
