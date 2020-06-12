#include "Assets.h"

#include "BuildConfig.h"
#include "Random.h"

// sdl2pp
#include <SDL2pp/SDL2pp.hh>

// std
#include <cassert>
#include <filesystem>

static constexpr int SECTION_COUNT = 10;
static constexpr int MIN_SECTION_LENGTH = 4;
static constexpr int MAX_SECTION_LENGTH = 15;

namespace fs = std::filesystem;

using namespace SDL2pp;
using namespace std;
using namespace Random;

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
        , player(loadMasked(renderer, "player"))
        , playerDown(loadMasked(renderer, "player-down"))
        , playerUp(loadMasked(renderer, "player-up"))
        , wall(loadMasked(renderer, "wall"))
        , bonuses(loadAllMasked(renderer, "bonus"))
        , textDrawer(textTexture, ALPHABET, CHAR_SIZE) {
    loadTileSets(renderer);
    loadSections();
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

enum TileId {
    BORDER = 0,
    ROAD0,
    ROAD1,
    EXTRA0,
    EXTRA1,
    EXTRA2,
};

static Section generateSection(const vector<TileSet>& tileSets, size_t columnCount) {
    Section section;
    const auto& tileSet = randomChoice(tileSets);
    for (size_t columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
        Section::Column column;

        // Bottom layer
        auto& bottomLayer = column.layers[0];
        std::fill(bottomLayer.begin(), bottomLayer.end(), &tileSet.tile(BORDER));
        auto it = bottomLayer.begin() + Section::BORDER_HEIGHT;
        auto end = bottomLayer.end() - Section::BORDER_HEIGHT;
        for (; it != end; ++it) {
            auto id = randomChoice<TileId>({ROAD0, ROAD1});
            *it = &tileSet.tile(id);
        }

        // Top layer
        auto& topLayer = column.layers[1];
        auto addRandomExtra = [&tileSet](const auto& it) {
            if (randomRange(3) == 0) {
                auto id = randomChoice<TileId>({EXTRA0, EXTRA1, EXTRA2});
                *it = &tileSet.tile(id);
            }
        };
        for (int idx = 0; idx < Section::BORDER_HEIGHT; ++idx) {
            addRandomExtra(topLayer.begin() + idx);
            addRandomExtra(topLayer.end() - idx - 1);
        }

        section.columns.push_back(column);
    }
    return section;
}

static Section loadSection(const vector<TileSet>& tileSets, const vector<string>& lines) {
    assert(lines.size() == LANE_COUNT);
    auto columnCount = lines.front().size();

    Section section = generateSection(tileSets, columnCount);

    for (size_t columnIdx = 0; columnIdx < columnCount; ++columnIdx) {
        auto& triggers = section.columns.at(columnIdx).triggers;
        for (size_t row = 0; row < lines.size(); ++row) {
            auto ch = lines.at(row).at(columnIdx);
            if (ch == '|') {
                triggers.at(row + Section::BORDER_HEIGHT) = TriggerId::Wall;
            } else if (ch == '*') {
                triggers.at(row + Section::BORDER_HEIGHT) = TriggerId::Bonus;
            }
        }
    }

    return section;
}

static void fillTriggers(Section::ColumnArray<TriggerId>& triggers) {
    int wallLane = -1;
    auto randomLaneRange = []() -> int {
        return randomRange(Section::BORDER_HEIGHT, Section::BORDER_HEIGHT + LANE_COUNT);
    };
    if (randomBool() == 0) {
        wallLane = randomLaneRange();
        triggers[wallLane] = TriggerId::Wall;
    }

    if (randomRange(4) == 0) {
        int bonusLane = wallLane;
        while (bonusLane == wallLane) {
            bonusLane = randomLaneRange();
        }
        triggers[bonusLane] = TriggerId::Bonus;
    }
}

void Assets::loadSections() {
    for (int i = 0; i < SECTION_COUNT; ++i) {
        int length = randomRange(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH);
        Section section = generateSection(tileSets, length);

        for (int columnIdx = 0; columnIdx < length; ++columnIdx) {
            if (columnIdx % SPAWN_SPACING == 0) {
                fillTriggers(section.columns.at(columnIdx).triggers);
            }
        }
        sections.emplace_back(section);
    }
    sections.emplace_back(loadSection(tileSets,
                                      {
                                          "||     ",
                                          "  ||   ",
                                          "    *  ",
                                          "  ||   ",
                                          "||     ",
                                      }));
    sections.emplace_back(loadSection(tileSets,
                                      {
                                          " *** *  * *** ",
                                          " * * ** *  *  ",
                                          " *** * **  *  ",
                                          " * * *  *  *  ",
                                          " * * *  *  *  ",
                                      }));
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
