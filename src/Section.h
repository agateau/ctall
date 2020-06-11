#ifndef SECTION_H
#define SECTION_H

#include "constants.h"

// std
#include <array>
#include <vector>

// + 2 for borders
static constexpr int SECTION_TILE_HEIGHT = LANE_COUNT + 2;

namespace SDL2pp {
class Texture;
class Point;
}

class World;

/**
 * Triggers something when the cell is about to become visible
 */
class Trigger {
public:
    virtual ~Trigger();
    virtual void exec(World& world, const SDL2pp::Point& position) const = 0;
};

template<class T>
using ColumnArray = std::array<T, SECTION_TILE_HEIGHT>;

struct Section {
    struct Column {
        const ColumnArray<const SDL2pp::Texture*> images;
        const ColumnArray<const Trigger*> triggers;
    };
    const std::vector<Column> columns;
};


#endif // SECTION_H
