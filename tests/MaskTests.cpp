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

using std::vector;

static Mask::Segment makeSegment(int start, int end) {
    return Mask::Segment{std::pair{start, end}};
}

TEST_CASE("Mask::fromStrings") {
    SECTION("dot") {
        Mask mask = Mask::fromStrings({"x"});
        CHECK(mask.horizontalSegments() == vector{makeSegment(0, 1)});
        CHECK(mask.verticalSegments() == vector{makeSegment(0, 1)});
    }
    SECTION("c") {
        Mask mask = Mask::fromStrings({" xx", // A weird `c' character
                                       "x  ",
                                       "xxx"});
        CHECK(mask.horizontalSegments()
              == vector{makeSegment(1, 3), makeSegment(0, 1), makeSegment(0, 3)});
        CHECK(mask.verticalSegments()
              == vector{makeSegment(1, 3), makeSegment(0, 3), makeSegment(0, 3)});
    }
    SECTION("4dots") {
        Mask mask = Mask::fromStrings({"x x", // A mask with an empty row and column
                                       "   ",
                                       "x x"});
        CHECK(mask.horizontalSegments() == vector{makeSegment(0, 3), {}, makeSegment(0, 3)});
        CHECK(mask.verticalSegments() == vector{makeSegment(0, 3), {}, makeSegment(0, 3)});
    }
}

TEST_CASE("Mask::collide") {
    Mask mask = Mask::fromStrings({"x   x", // A cross
                                   " x x ",
                                   "  x  ",
                                   " x x ",
                                   "x   x"});
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
