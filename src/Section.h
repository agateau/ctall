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
    const SDL2pp::Texture* image;
    const SDL2pp::Point pos = {0, 0};
};

class TileSet {
public:
    enum TileId {
        BORDER = 0,
        ROAD0 = 1,
        ROAD1 = 2
    };
    explicit TileSet(std::unique_ptr<SDL2pp::Texture> tileImage);

    Tile* tile(int tileId) const;

private:
    std::unique_ptr<SDL2pp::Texture> mTileImage;
    std::vector<std::unique_ptr<Tile>> mTiles;
};

struct Section {
    struct Column {
        const ColumnArray<const Tile*> tiles;
        ColumnArray<const Trigger*> triggers;
    };
    std::vector<Column> columns;
};

#endif // SECTION_H
