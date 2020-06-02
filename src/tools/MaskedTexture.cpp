#include "MaskedTexture.h"

using namespace SDL2pp;

MaskedTexture::MaskedTexture(Renderer& renderer, const std::string& filename)
        : MaskedTexture(renderer, Surface(filename)) {
}

MaskedTexture::MaskedTexture(Renderer& renderer, const SDL2pp::Surface& surface)
        : texture(renderer, surface), mask(Mask::fromSurfaceAlpha(surface)) {
}
