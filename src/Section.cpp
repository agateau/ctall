#include "Section.h"

using namespace SDL2pp;
using namespace std;

Trigger::~Trigger() = default;

TileSet::TileSet(unique_ptr<Texture> tileImage) : mTileImage(move(tileImage)) {
    auto* image = mTileImage.get();
    const auto imageSize = image->GetSize();

    for (int y = 0; y < imageSize.y; y += TILE_SIZE) {
        for (int x = 0; x < imageSize.x; x += TILE_SIZE) {
            auto tile = make_unique<Tile>(Tile{image, {x, y}});
            mTiles.emplace_back(std::move(tile));
        }
    }
}

Tile* TileSet::tile(int tileId) const {
    assert(tileId <= ROAD1);
    return mTiles.at(tileId).get();
}
