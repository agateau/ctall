#ifndef SECTION_H
#define SECTION_H

#include "constants.h"

// SDL2pp
#include <SDL2pp/SDL2pp.hh>

// std
#include <array>
#include <memory>
#include <vector>

// + 2 for borders
static constexpr int SECTION_TILE_HEIGHT = LANE_COUNT + 2;

class World;

/**
 * Triggers something when the cell is about to become visible
 */
class Trigger {
public:
    virtual ~Trigger();
    virtual void exec(World& world, const SDL2pp::Point& position) const = 0;
};

template <class T> using ColumnArray = std::array<T, SECTION_TILE_HEIGHT>;

struct Tile {
    const SDL2pp::Texture& image;
    const SDL2pp::Point pos = {0, 0};
};

class TileSet {
public:
    explicit TileSet(std::unique_ptr<SDL2pp::Texture> tileImage);

    const Tile& tile(std::size_t tileId) const;

private:
    std::unique_ptr<SDL2pp::Texture> mTileImage;
    std::vector<Tile> mTiles;
};

struct Section {
    struct Column {
        Column();
        std::array<ColumnArray<const Tile*>, 2> layers;
        ColumnArray<const Trigger*> triggers;
    };
    std::vector<Column> columns;
};

#endif // SECTION_H
