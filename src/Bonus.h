#ifndef BONUS_H
#define BONUS_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"
#include "ScrollComponent.h"

class Game;

template <class T> class Pool;

class Bonus : public GameObject {
public:
    Bonus(Game& game,
          Pool<Bonus>& pool,
          const Scroller& scroller,
          std::vector<SDL2pp::Texture>& textures);

    void setup(int lane);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

    void onCaptured();

private:
    Game& mGame;
    Pool<Bonus>& mPool;
    std::vector<SDL2pp::Texture>& mTextures;
    ScrollComponent mScrollComponent;
    SDL2pp::Texture* mActiveTexture = nullptr;
};

#endif // BONUS_H
