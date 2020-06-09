#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"

struct Input;
class World;
class MaskedTexture;

class Player : public GameObject {
public:
    Player(World& world,
           MaskedTexture& texture,
           MaskedTexture& upTexture,
           MaskedTexture& downTexture,
           const Input& input);

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

    int capturedCount() const {
        return mCapturedCount;
    }

private:
    void updateTargetLane();
    void updateY(float delta);
    void updateTexture();
    void checkCollisions();
    void setActiveTexture(MaskedTexture* texture);

    World& mWorld;
    MaskedTexture& mTexture;
    MaskedTexture& mUpTexture;
    MaskedTexture& mDownTexture;
    MaskedTexture* mActiveTexture;
    const Input& mInput;
    int mCapturedCount = 0;
};

#endif // PLAYER_H
