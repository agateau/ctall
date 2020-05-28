#include "Assets.h"

#include <SDL2pp/SDL2pp.hh>
#include <cassert>

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
        , player(load(renderer, "player"))
        , wall(load(renderer, "wall"))
        , textDrawer(textTexture, ALPHABET, CHAR_SIZE) {
}

Texture Assets::load(Renderer& renderer, const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    return Texture(renderer, path);
}
