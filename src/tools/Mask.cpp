#include "Mask.h"

#include <SDL2pp/SDL2pp.hh>

// std
#include <algorithm>
#include <cassert>

static constexpr char FILLED_CHAR = 'x';

Mask::Mask() {
}

inline bool checkSegments(const std::vector<Mask::Segment>& segments1,
                          const std::vector<Mask::Segment>& segments2,
                          int offsetU, int offsetV) {
    auto it1 = segments1.cbegin();
    auto it2 = segments2.cbegin();
    if (offsetU > 0) {
        it1 += offsetU;
    } else {
        it2 += offsetU;
    }
    auto end1 = segments1.cend();
    auto end2 = segments1.cend();
    for (; it1 != end1 && it2 != end2; ++it1, ++it2) {
        if (!it1->has_value() || !it2->has_value()) {
            continue;
        }
        auto [start1, end1] = it1->value();
        auto [start2, end2] = it2->value();
        start2 += offsetV;
        end2 += offsetV;
        if (start1 <= start2 && start2 < end1) {
            return true;
        }
        if (start2 <= start1 && start1 < end2) {
            return true;
        }
    }
    return false;
}

bool Mask::collide(const Mask& mask1, const Mask& mask2, const SDL2pp::Point& offset) {
    return checkSegments(mask1.horizontalSegments(), mask2.horizontalSegments(), offset.x, offset.y)
           && checkSegments(mask1.verticalSegments(), mask2.verticalSegments(), offset.y, offset.x);
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
