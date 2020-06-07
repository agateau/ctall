#include <SDL2pp/SDL2pp.hh>
#include <fstream>

#include "Mask.h"

std::ostream& operator<<(std::ostream& ostr, const Mask::Segment& segment) {
    if (segment.has_value()) {
        auto pair = segment.value();
        ostr << '(' << pair.first << ", " << pair.second << ')';
    } else {
        ostr << "(none)";
    }
    return ostr;
}

#include <catch2/catch.hpp>

using namespace std;
using namespace SDL2pp;

static constexpr char FILLED_CHAR = 'x';

static Mask::Segment makeSegment(int start, int end) {
    return Mask::Segment{std::pair{start, end}};
}

static Surface surfaceFromStrings(const std::vector<std::string>& strings) {
    assert(!strings.empty());
    auto width = strings.at(0).size();
    auto height = strings.size();

    auto pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    int bpp;
    Uint32 rmask, gmask, bmask, amask;
    SDL_PixelFormatEnumToMasks(pixelFormat, &bpp, &rmask, &gmask, &bmask, &amask);

    Surface surface(0 /* flags */, int(width), int(height), bpp, rmask, gmask, bmask, amask);

    Surface::LockHandle lock = surface.Lock();
    auto pixelBytes = reinterpret_cast<Uint8*>(lock.GetPixels());
    memset(pixelBytes, 0, width * height * 4);

    for (size_t y = 0; y < height; ++y) {
        Uint32* pixels = reinterpret_cast<Uint32*>(pixelBytes + y * lock.GetPitch());
        for (size_t x = 0; x < width; ++x, ++pixels) {
            if (strings.at(y).at(x) == FILLED_CHAR) {
                *pixels = 0xffffffff;
            }
        }
    }
    return surface;
}

TEST_CASE("Mask::fromSurfaceAlpha") {
    SECTION("dot") {
        Surface surface = surfaceFromStrings({"x"});
        Mask mask = Mask::fromSurfaceAlpha(surface);
        CHECK(mask.horizontalSegments() == vector{makeSegment(0, 1)});
        CHECK(mask.verticalSegments() == vector{makeSegment(0, 1)});
    }
    SECTION("c") {
        Surface surface = surfaceFromStrings({" xx", // A weird `c' character
                                              "x  ",
                                              "xxx"});
        Mask mask = Mask::fromSurfaceAlpha(surface);
        CHECK(mask.horizontalSegments()
              == vector{makeSegment(1, 3), makeSegment(0, 1), makeSegment(0, 3)});
        CHECK(mask.verticalSegments()
              == vector{makeSegment(1, 3), makeSegment(0, 3), makeSegment(0, 3)});
    }
    SECTION("4dots") {
        Surface surface = surfaceFromStrings({"x x", // A mask with an empty row and column
                                              "   ",
                                              "x x"});
        Mask mask = Mask::fromSurfaceAlpha(surface);
        CHECK(mask.horizontalSegments() == vector{makeSegment(0, 3), {}, makeSegment(0, 3)});
        CHECK(mask.verticalSegments() == vector{makeSegment(0, 3), {}, makeSegment(0, 3)});
    }
}

TEST_CASE("Mask::collideX") {
    Surface surface = surfaceFromStrings({"x   x", // A cross
                                          " x x ",
                                          "  x  ",
                                          " x x ",
                                          "x   x"});
    Mask mask = Mask::fromSurfaceAlpha(surface);
    SECTION("horizontal") {
        REQUIRE(Mask::collide(mask, mask, {2, 0}));
        REQUIRE(Mask::collide(mask, mask, {-2, 0}));
    }
    SECTION("vertical") {
        REQUIRE(Mask::collide(mask, mask, {0, 2}));
        REQUIRE(Mask::collide(mask, mask, {0, -2}));
    }
    SECTION("noHitHorizontal") {
        // Top-left corner of 2nd mask is between the top-right and bottom-right corners of first
        // mask
        REQUIRE_FALSE(Mask::collide(mask, mask, {4, 2}));
    }
    SECTION("noHitVertical") {
        // Top-left corner of 2nd mask is between the bottom-left and bottom-right corners of first
        // mask
        REQUIRE_FALSE(Mask::collide(mask, mask, {2, 4}));
    }
}

TEST_CASE("Mask::collideIU") {
    Surface uSurface = surfaceFromStrings({"x x", // A U
                                           "x x",
                                           "xxx"});
    Mask uMask = Mask::fromSurfaceAlpha(uSurface);

    Surface iSurface = surfaceFromStrings({"x", // A I
                                           "x"});
    Mask iMask = Mask::fromSurfaceAlpha(iSurface);

    SECTION("iInsideU") {
        CHECK(Mask::collide(uMask, iMask, {0, 0}));
        CHECK_FALSE(Mask::collide(uMask, iMask, {1, 0}));
        CHECK(Mask::collide(uMask, iMask, {2, 0}));
    }
}

TEST_CASE("Mask::collideC-") {
    Surface cSurface = surfaceFromStrings({"xxx", // A C
                                           "x  ",
                                           "xxx"});
    Mask cMask = Mask::fromSurfaceAlpha(cSurface);

    Surface dashSurface = surfaceFromStrings({"xx"}); // A -
    Mask dashMask = Mask::fromSurfaceAlpha(dashSurface);

    SECTION("dashInsideU") {
        CHECK(Mask::collide(cMask, dashMask, {1, 0}));
        CHECK_FALSE(Mask::collide(cMask, dashMask, {1, 1}));
        CHECK(Mask::collide(cMask, dashMask, {1, 2}));
    }

    SECTION("dashInsideUSwap") {
        CHECK(Mask::collide(dashMask, cMask, {-1, 0}));
        CHECK_FALSE(Mask::collide(dashMask, cMask, {-1, -1}));
        CHECK(Mask::collide(dashMask, cMask, {-1, -2}));
    }
}
