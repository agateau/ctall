#include "MaskedTexture.h"

using namespace SDL2pp;

MaskedTexture::MaskedTexture(Renderer& renderer, const std::string& filename)
        : MaskedTexture(renderer, Surface(filename)) {
}

MaskedTexture::MaskedTexture(Renderer& renderer,
                             const std::string& filename,
                             const std::string& maskFilename)
        : MaskedTexture(renderer, Surface(filename), Surface(maskFilename)) {
}

MaskedTexture::MaskedTexture(Renderer& renderer, const SDL2pp::Surface& surface)
        : MaskedTexture(renderer, surface, surface) {
}

MaskedTexture::MaskedTexture(Renderer& renderer,
                             const SDL2pp::Surface& surface,
                             const Surface& surfaceMask)
        : texture(renderer, surface), mask(Mask::fromSurfaceAlpha(surfaceMask)) {
}
