#ifndef MASKEDTEXTURE_H
#define MASKEDTEXTURE_H

#include <SDL2pp/SDL2pp.hh>
#include <string>

#include "Mask.h"

class Mask;

class MaskedTexture {
public:
    MaskedTexture(SDL2pp::Renderer& renderer, const std::string& filename);
    MaskedTexture(SDL2pp::Renderer& renderer, const std::string& filename, const std::string& maskFilename);
    MaskedTexture(SDL2pp::Renderer& renderer, const SDL2pp::Surface& surface);
    MaskedTexture(SDL2pp::Renderer& renderer,
                  const SDL2pp::Surface& surface,
                  const SDL2pp::Surface& surfaceMask);

    SDL2pp::Texture texture;
    const Mask mask;
};

#endif // MASKEDTEXTURE_H
