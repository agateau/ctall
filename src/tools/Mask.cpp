#include "Mask.h"

#include <algorithm>
#include <cassert>

static constexpr char FILLED_CHAR = 'x';

Mask::Mask() {
}

bool Mask::collide(const Mask& mask1, const Mask& mask2, const SDL2pp::Point& offset) {
    return true;
}

Mask Mask::fromStrings(const std::vector<std::string>& strings) {
    Mask mask;

    assert(!strings.empty());
    auto width = strings.at(0).size();
    auto height = strings.size();

    for (const auto& line : strings) {
        auto start = line.find(FILLED_CHAR);
        if (start == std::string::npos) {
            mask.mHorizontalSegments.push_back({});
            continue;
        }
        auto end = line.rfind(FILLED_CHAR);
        Segment segment = std::pair{start, end + 1};
        mask.mHorizontalSegments.push_back(segment);
    }

    for (std::size_t x = 0; x < width; ++x) {
        std::size_t start = 0;
        for (; start < height; ++start) {
            if (strings.at(start).at(x) == FILLED_CHAR) {
                break;
            }
        }
        if (start == height) {
            mask.mVerticalSegments.push_back({});
            continue;
        }
        auto end = height - 1;
        for (; end > start; --end) {
            if (strings.at(end).at(x) == FILLED_CHAR) {
                break;
            }
        }
        Segment segment = std::pair{start, end + 1};
        mask.mVerticalSegments.push_back(segment);
    }

    return mask;
}
