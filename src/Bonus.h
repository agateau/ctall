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
    Bonus(Pool<Bonus>& pool,
          const Scroller& scroller,
          std::vector<MaskedTexture>& textures);

    void setup(const SDL2pp::Point& pos);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

    void onCaptured();

private:
    Pool<Bonus>& mPool;
    std::vector<MaskedTexture>& mTextures;
    ScrollComponent mScrollComponent;
    MaskedTexture* mActiveTexture = nullptr;
};

#endif // BONUS_H
