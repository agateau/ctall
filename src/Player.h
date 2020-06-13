#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"
#include "MaskedTexture.h"

struct Input;
class World;

struct PlayerTextures {
    MaskedTexture normal;
    MaskedTexture up;
    MaskedTexture down;
};

class Player : public GameObject {
public:
    Player(World& world, const PlayerTextures& textures, const Input& input);

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
    void setActiveTexture(const MaskedTexture* texture);

    World& mWorld;
    const PlayerTextures& mTextures;
    const MaskedTexture* mActiveTexture;
    const Input& mInput;
    int mCapturedCount = 0;
};

#endif // PLAYER_H
