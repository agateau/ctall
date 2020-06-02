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

static constexpr Point CHAR_SIZE = {18, 16};

Assets::Assets(Renderer& renderer)
        : mBaseDir("images")
        , textTexture(load(renderer, "font"))
        , player(loadMasked(renderer, "player"))
        , wall(loadMasked(renderer, "wall"))
        , bonuses(loadAllMasked(renderer, "bonus"))
        , textDrawer(textTexture, ALPHABET, CHAR_SIZE) {
    loadBackgrounds(renderer);
}

void Assets::loadBackgrounds(Renderer& renderer) {
    auto loadBackground = [&renderer](const fs::path& backgroundDir) {
        Texture border = Texture(renderer, backgroundDir / "border.png");
        std::vector<Texture> roads;
        for (int idx = 0;; ++idx) {
            auto roadPath = backgroundDir / ("road-" + std::to_string(idx) + ".png");
            if (!fs::is_regular_file(roadPath)) {
                break;
            }
            roads.emplace_back(Texture(renderer, roadPath));
        }
        assert(!roads.empty());
        return BackgroundAssets{
            std::move(border),
            std::move(roads)
        };
    };
    auto backgroundsDir = fs::path(mBaseDir) / "backgrounds";
    for (int idx = 0;; ++idx) {
        auto backgroundDir = backgroundsDir / std::to_string(idx);
        if (!fs::is_directory(backgroundDir)) {
            break;
        }
        backgrounds.emplace_back(loadBackground(backgroundDir));
    }
    assert(!backgrounds.empty());
}

Texture Assets::load(Renderer& renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    return Texture(renderer, path);
}

MaskedTexture Assets::loadMasked(Renderer& renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    return MaskedTexture(renderer, path);
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
