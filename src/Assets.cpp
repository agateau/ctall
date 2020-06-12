#include "Assets.h"

#include <SDL2pp/SDL2pp.hh>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

using namespace SDL2pp;

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
        : mBaseDir("assets")
        , textTexture(load(renderer, "font"))
        , player(loadMasked(renderer, "player"))
        , playerDown(loadMasked(renderer, "player-down"))
        , playerUp(loadMasked(renderer, "player-up"))
        , wall(loadMasked(renderer, "wall"))
        , bonuses(loadAllMasked(renderer, "bonus"))
        , textDrawer(textTexture, ALPHABET, CHAR_SIZE) {
    loadTileSets(renderer);
}

void Assets::loadTileSets(Renderer& renderer) {
    auto backgroundsDir = fs::path(mBaseDir) / "backgrounds";
    for (int idx = 0;; ++idx) {
        auto tileImagePath = backgroundsDir / (std::to_string(idx) + ".png");
        if (!fs::is_regular_file(tileImagePath)) {
            break;
        }
        auto tileImage = std::make_unique<Texture>(renderer, tileImagePath);
        tileSets.emplace_back(TileSet(std::move(tileImage)));
    }
    assert(!tileSets.empty());
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
