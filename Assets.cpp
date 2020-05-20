#include "Assets.h"

#include <cassert>

Assets::Assets() {
    mBaseDir = "images";
    player = load("player");
    wall = load("wall");
}

sf::Texture Assets::load(const std::string& name) {
    auto path = mBaseDir + "/" + name + ".png";
    sf::Texture texture;
    bool ok = texture.loadFromFile(path);
    assert(ok);
    return texture;
}
