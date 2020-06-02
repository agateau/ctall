#ifndef MASKEDTEXTURE_H
#define MASKEDTEXTURE_H

#include <string>

#include "Mask.h"

#include <SDL2pp/SDL2pp.hh>

class Mask;

class MaskedTexture {
public:
    MaskedTexture(SDL2pp::Renderer& renderer, const std::string& filename);
    MaskedTexture(SDL2pp::Renderer& renderer, const SDL2pp::Surface& surface);

    SDL2pp::Texture texture;
    const Mask mask;
};

#endif // MASKEDTEXTURE_H
