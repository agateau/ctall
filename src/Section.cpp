#include "Section.h"

using namespace SDL2pp;
using namespace std;

Trigger::~Trigger() = default;

TileSet::TileSet(unique_ptr<Texture> tileImage) : mTileImage(move(tileImage)) {
    auto* image = mTileImage.get();
    Point point = {0, 0};

    for (int i = 0; i <= ROAD1; ++i, point.x += TILE_SIZE) {
        auto tile = make_unique<Tile>(Tile{image, point});
        mTiles.emplace_back(std::move(tile));
    }
}

Tile* TileSet::tile(int tileId) const {
    assert(tileId <= ROAD1);
    return mTiles.at(tileId).get();
}
