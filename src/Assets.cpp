#include "Assets.h"

#include "BuildConfig.h"

// sdl2pp
#include <SDL2pp/SDL2pp.hh>

// tmxlite
#include <tmxlite/Map.hpp>
#include <tmxlite/Object.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/TileLayer.hpp>

// std
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

using namespace SDL2pp;
using namespace std;

static constexpr char ALPHABET[] = " !*+,-./0"
                                   "123\"45678"
                                   "9:;<=#>?@"
                                   "ABCDEFG$H"
                                   "IJKLMNOPQ"
                                   "%RSTUVWXY"
                                   "2[&\\]^_`'"
                                   "(){|}~   ";

static constexpr Point CHAR_SIZE = {9, 8};

Assets::Assets(Renderer& renderer)
        : mBaseDir(ASSETS_DIR)
        , textTexture(load(renderer, "font"))
        , player({loadMasked(renderer, "player-1"),
                  loadMasked(renderer, "player-up-1"),
                  loadMasked(renderer, "player-down-1")})
        , wall(loadMasked(renderer, "wall"))
        , bonuses(loadAllMasked(renderer, "bonus"))
        , textDrawer(textTexture, ALPHABET, CHAR_SIZE) {
    loadTileSets(renderer);
    loadSections(renderer);
}

void Assets::loadTileSets(Renderer& renderer) {
    auto backgroundsDir = fs::path(mBaseDir) / "backgrounds";
    for (int idx = 0;; ++idx) {
        auto tileImagePath = backgroundsDir / (std::to_string(idx) + ".png");
        if (!fs::is_regular_file(tileImagePath)) {
            break;
        }
        loadTileSet(renderer, tileImagePath, 0);
    }
}

const TileSet& Assets::loadTileSet(Renderer& renderer, const string& path, int spacing) {
    if (auto it = mTileSets.find(path); it != mTileSets.end()) {
        return it->second;
    }
    assert(fs::is_regular_file(path));
    auto tileImage = std::make_unique<Texture>(renderer, path);
    auto [it, inserted] = mTileSets.emplace(path, TileSet(std::move(tileImage), spacing));
    return it->second;
}

unique_ptr<Section> Assets::loadSection(SDL2pp::Renderer& renderer, const string& tmxPath) {
    auto section = make_unique<Section>();

    assert(fs::is_regular_file(tmxPath));
    tmx::Map map;
    if (!map.load(tmxPath)) {
        abort();
    }

    assert(map.getTilesets().size() == 1);
    auto tmxTileSet = map.getTilesets().front();
    const auto& tileSet = loadTileSet(renderer, tmxTileSet.getImagePath(), tmxTileSet.getSpacing());

    auto mapTileCount = map.getTileCount();
    assert(mapTileCount.y == Section::TOTAL_HEIGHT);
    section->columns.resize(mapTileCount.x);

    const auto& layers = map.getLayers();
    assert(layers.size() == Section::IMAGE_LAYERS + 1);

    // Fill image layers
    for (size_t layerIdx = 0; layerIdx < Section::IMAGE_LAYERS; ++layerIdx) {
        const auto& layer = layers.at(layerIdx);
        assert(layer->getType() == tmx::Layer::Type::Tile);
        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();

        for (size_t columnIdx = 0; columnIdx < mapTileCount.x; ++columnIdx) {
            auto& column = section->columns.at(columnIdx);
            for (size_t rowIdx = 0; rowIdx < mapTileCount.y; ++rowIdx) {
                size_t tileIdx = rowIdx * mapTileCount.x + columnIdx;
                auto tmxTile = tileLayer.getTiles().at(tileIdx);
                if (tmxTile.ID == 0) {
                    continue;
                }
                const auto& tile = tileSet.tile(tmxTile.ID - 1);
                column.layers.at(layerIdx)[rowIdx] = &tile;
            }
        }
    }

    // Fill triggers
    const auto& layer = layers.at(Section::IMAGE_LAYERS);
    assert(layer->getType() == tmx::Layer::Type::Object);
    const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
    for (const auto& object : objectLayer.getObjects()) {
        auto type = object.getType();
        TriggerId triggerId = TriggerId::None;
        if (type == "Wall") {
            triggerId = TriggerId::Wall;
        } else if (type == "Bonus") {
            triggerId = TriggerId::Bonus;
        } else {
            cerr << "Invalid object type: " << type << '\n';
            abort();
        }
        size_t columnIdx = size_t(object.getPosition().x / TILE_SIZE);
        size_t rowIdx = size_t(object.getPosition().y / TILE_SIZE);
        section->columns.at(columnIdx).triggers[rowIdx] = triggerId;
    }

    return section;
}

void Assets::loadSections(SDL2pp::Renderer& renderer) {
    auto dirPath = mBaseDir + "/backgrounds";
    for (const fs::directory_entry& entry : fs::directory_iterator(dirPath)) {
        if (entry.path().extension() == ".tmx") {
            sections.push_back(loadSection(renderer, entry.path()));
            sectionsByName[entry.path().stem()] = sections.back().get();
        }
    }
}

Texture Assets::load(Renderer& renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    return Texture(renderer, path);
}

MaskedTexture Assets::loadMasked(Renderer& renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    auto maskPath = mBaseDir + "/" + name + "-mask.png";
    if (fs::is_regular_file(maskPath)) {
        return MaskedTexture(renderer, path, maskPath);
    } else {
        return MaskedTexture(renderer, path);
    }
}

std::vector<MaskedTexture> Assets::loadAllMasked(Renderer& renderer, const std::string& dirName) {
    auto dirPath = mBaseDir + "/" + dirName;
    std::vector<MaskedTexture> vec;

    for (const fs::directory_entry& entry : fs::directory_iterator(dirPath)) {
        if (entry.path().extension() == ".png") {
            vec.emplace_back(MaskedTexture(renderer, entry.path()));
        }
    }
    return vec;
}
