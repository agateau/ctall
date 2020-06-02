#include "Mask.h"

#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

Mask::Mask() {
}

inline bool checkSegments(const std::vector<Mask::Segment>& segments1,
                          const std::vector<Mask::Segment>& segments2,
                          int offsetU,
                          int offsetV) {
    auto it1 = segments1.cbegin();
    auto it2 = segments2.cbegin();
    if (offsetV > 0) {
        it1 += offsetV;
    } else {
        it2 += offsetV;
    }
    auto end1 = segments1.cend();
    auto end2 = segments2.cend();
    for (; it1 != end1 && it2 != end2; ++it1, ++it2) {
        if (!it1->has_value() || !it2->has_value()) {
            continue;
        }
        auto [start1, stop1] = it1->value();
        auto [start2, stop2] = it2->value();
        start2 += offsetU;
        stop2 += offsetU;
        if (start1 <= start2 && start2 < stop1) {
            return true;
        }
        if (start2 <= start1 && start1 < stop2) {
            return true;
        }
    }
    return false;
}

#include <iostream>

bool Mask::collide(const Mask& mask1, const Mask& mask2, const SDL2pp::Point& offset) {
    return checkSegments(mask1.horizontalSegments(), mask2.horizontalSegments(), offset.x, offset.y)
           && checkSegments(mask1.verticalSegments(), mask2.verticalSegments(), offset.y, offset.x);
}

Mask Mask::fromSurfaceAlpha(Surface& surface_) {
    Mask mask;

    const auto pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    Surface surface = surface_.Convert(pixelFormat);

    int bpp;
    Uint32 rmask, gmask, bmask, amask;
    SDL_PixelFormatEnumToMasks(pixelFormat, &bpp, &rmask, &gmask, &bmask, &amask);

    Surface::LockHandle lock = surface.Lock();
    auto [width, height] = surface.GetSize();

    auto isFilled = [&lock, amask](int x, int y) {
        auto pixelBytes = reinterpret_cast<Uint8*>(lock.GetPixels());
        Uint32* pixel = reinterpret_cast<Uint32*>(pixelBytes + y * lock.GetPitch()) + x;
        return (*pixel & amask) > 0;
    };

    for (int y = 0; y < height; ++y) {
        int start = 0;
        for (; start < width; ++start) {
            if (isFilled(start, y)) {
                break;
            }
        }
        if (start == width) {
            mask.mHorizontalSegments.push_back({});
            continue;
        }
        auto stop = width - 1;
        for (; stop > start; --stop) {
            if (isFilled(stop, y)) {
                break;
            }
        }
        Segment segment = std::pair{start, stop + 1};
        mask.mHorizontalSegments.push_back(segment);
    }

    for (int x = 0; x < width; ++x) {
        int start = 0;
        for (; start < height; ++start) {
            if (isFilled(x, start)) {
                break;
            }
        }
        if (start == height) {
            mask.mVerticalSegments.push_back({});
            continue;
        }
        auto stop = height - 1;
        for (; stop > start; --stop) {
            if (isFilled(x, stop)) {
                break;
            }
        }
        Segment segment = std::pair{start, stop + 1};
        mask.mVerticalSegments.push_back(segment);
    }

    return mask;
}
