#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"

struct Input;
class Game;

class Player : public GameObject {
public:
    Player(Game& game, SDL2pp::Texture& texture, const Input& input);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

private:
    Game& mGame;
    SDL2pp::Texture& mTexture;
    const Input& mInput;
    int mCurrentLane = 0;
    int mTargetLane = 0;
};

#endif // PLAYER_H
