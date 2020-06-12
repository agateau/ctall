#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2pp/SDL2pp.hh>

#include "MaskedTexture.h"
#include "Section.h"
#include "TextDrawer.h"

// std
#include <memory>
#include <unordered_map>

struct Assets {
private:
    std::string mBaseDir;
    SDL2pp::Texture textTexture;

public:
    Assets(SDL2pp::Renderer& renderer);

    MaskedTexture player;
    MaskedTexture playerDown;
    MaskedTexture playerUp;
    MaskedTexture wall;
    std::vector<MaskedTexture> bonuses;
    TextDrawer textDrawer;
    std::vector<Section> sections;

private:
    void loadTileSets(SDL2pp::Renderer& renderer);
    void loadTileSet(SDL2pp::Renderer& renderer, const std::string& path, int spacing);

    void loadSections();

    Section loadSection(const std::string& tmxPath);

    std::unordered_map<std::string, TileSet> mTileSets;

    SDL2pp::Texture load(SDL2pp::Renderer& renderer, const std::string& name);
    MaskedTexture loadMasked(SDL2pp::Renderer& renderer, const std::string& name);
    std::vector<MaskedTexture> loadAllMasked(SDL2pp::Renderer& renderer,
                                             const std::string& dirName);
};

#endif // ASSETS_H
