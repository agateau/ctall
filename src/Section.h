#ifndef SECTION_H
#define SECTION_H

#include "constants.h"

// SDL2pp
#include <SDL2pp/SDL2pp.hh>

// std
#include <array>
#include <memory>
#include <vector>

class World;

enum class TriggerId {
    None,
    Wall,
    Bonus
};

/**
 * Triggers something when the cell is about to become visible
 */
class Trigger {
public:
    virtual ~Trigger();
    virtual void exec(World& world, const SDL2pp::Point& position) const = 0;
};

struct Tile {
    const SDL2pp::Texture& image;
    const SDL2pp::Point pos = {0, 0};
};

class TileSet {
public:
    explicit TileSet(std::unique_ptr<SDL2pp::Texture> tileImage, int spacing);

    const Tile& tile(std::size_t tileId) const;

private:
    std::unique_ptr<SDL2pp::Texture> mTileImage;
    std::vector<Tile> mTiles;
};

struct Section {
    static constexpr int BORDER_HEIGHT = 2;
    static constexpr int TOTAL_HEIGHT = LANE_COUNT + 2 * BORDER_HEIGHT;
    static constexpr int IMAGE_LAYERS = 2;

    template <class T> using ColumnArray = std::array<T, TOTAL_HEIGHT>;

    struct Column {
        Column();
        std::array<ColumnArray<const Tile*>, IMAGE_LAYERS> layers;
        ColumnArray<TriggerId> triggers;
    };
    std::vector<Column> columns;
};

#endif // SECTION_H
