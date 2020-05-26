#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

#include <SDL2pp/SDL2pp.hh>

struct Input;

class Player : public GameObject {
public:
    Player(SDL2pp::Texture& texture, const Input& input);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

private:
    SDL2pp::Texture& mTexture;
    const Input& mInput;
    SDL2pp::Point mPos;
};


#endif // PLAYER_H
