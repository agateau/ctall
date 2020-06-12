#include "Section.h"

using namespace SDL2pp;
using namespace std;

Trigger::~Trigger() = default;

TileSet::TileSet(unique_ptr<Texture> tileImage, int spacing) : mTileImage(move(tileImage)) {
    const auto imageSize = mTileImage->GetSize();

    for (int y = 0; y < imageSize.y; y += TILE_SIZE + spacing) {
        for (int x = 0; x < imageSize.x; x += TILE_SIZE + spacing) {
            mTiles.emplace_back(Tile{*mTileImage.get(), {x, y}});
        }
    }
}

const Tile& TileSet::tile(size_t tileId) const {
    assert(tileId < mTiles.size());
    return mTiles.at(tileId);
}

Section::Column::Column() {
    for (auto& layer : layers) {
        std::fill(layer.begin(), layer.end(), nullptr);
    }
    std::fill(triggers.begin(), triggers.end(), TriggerId::None);
}
