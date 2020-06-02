#ifndef MASK_H
#define MASK_H

#include <optional>
#include <vector>

namespace SDL2pp {
class Point;
class Surface;
}

class Mask {
public:
    using Segment = std::optional<std::pair<int, int>>;

    Mask();

    static bool collide(const Mask& mask1, const Mask& mask2, const SDL2pp::Point& offset);

    static Mask fromSurfaceAlpha(const SDL2pp::Surface& surface);

    const std::vector<Segment>& horizontalSegments() const {
        return mHorizontalSegments;
    }

    const std::vector<Segment>& verticalSegments() const {
        return mVerticalSegments;
    }

private:
    std::vector<Segment> mHorizontalSegments;
    std::vector<Segment> mVerticalSegments;
};

#endif // MASK_H
