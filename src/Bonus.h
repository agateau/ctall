#ifndef BONUS_H
#define BONUS_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"
#include "MaskedTexture.h"
#include "ScrollComponent.h"

class World;

template <class T> class Pool;

class Bonus : public GameObject {
public:
    Bonus(const World& world,
          Pool<Bonus>& pool,
          const Scroller& scroller,
          std::vector<MaskedTexture>& textures);

    void setup(int lane);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

    void onCaptured();

private:
    const World& mWorld;
    Pool<Bonus>& mPool;
    std::vector<MaskedTexture>& mTextures;
    ScrollComponent mScrollComponent;
    MaskedTexture* mActiveTexture = nullptr;
};

#endif // BONUS_H
