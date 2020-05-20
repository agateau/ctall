#ifndef ASSETS_H
#define ASSETS_H

#include <SFML/Graphics.hpp>

struct Assets {
    Assets();

    sf::Texture player;
    sf::Texture wall;

private:
    std::string mBaseDir;

    sf::Texture load(const std::string& name);
};

#endif // ASSETS_H
