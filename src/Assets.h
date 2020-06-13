#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2pp/SDL2pp.hh>

#include "MaskedTexture.h"
#include "Player.h"
#include "Section.h"
#include "TextDrawer.h"

// std
#include <memory>
#include <unordered_map>

static constexpr int AVAILABLE_SKATER_COUNT = 2;

struct Assets {
private:
    std::string mBaseDir;
    SDL2pp::Texture textTexture;

public:
    Assets(SDL2pp::Renderer& renderer);

    std::vector<PlayerTextures> players;
    std::array<std::string, AVAILABLE_SKATER_COUNT> skaterNames = {
        "BOB", "DINA"
    };
    MaskedTexture wall;
    std::vector<MaskedTexture> bonuses;
    TextDrawer textDrawer;
    std::vector<std::unique_ptr<Section>> sections;

    std::unordered_map<std::string, const Section*> sectionsByName;

private:
    void loadTileSets(SDL2pp::Renderer& renderer);
    const TileSet& loadTileSet(SDL2pp::Renderer& renderer, const std::string& path, int spacing);

    void loadSections(SDL2pp::Renderer& renderer);

    std::unique_ptr<Section> loadSection(SDL2pp::Renderer& renderer, const std::string& tmxPath);

    std::unordered_map<std::string, TileSet> mTileSets;

    SDL2pp::Texture load(SDL2pp::Renderer& renderer, const std::string& name);
    MaskedTexture loadMasked(SDL2pp::Renderer& renderer, const std::string& name);
    std::vector<MaskedTexture> loadAllMasked(SDL2pp::Renderer& renderer,
                                             const std::string& dirName);

    void loadPlayerTextures(SDL2pp::Renderer& renderer);
    void loadPlayerTexture(SDL2pp::Renderer& renderer, int idx);
};

#endif // ASSETS_H
