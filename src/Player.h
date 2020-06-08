#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2pp/SDL2pp.hh>

#include "GameObject.h"

struct Input;
class GameScreen;
class MaskedTexture;

class Player : public GameObject {
public:
    Player(GameScreen& game,
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

    GameScreen& mGameScreen;
    MaskedTexture& mTexture;
    MaskedTexture& mUpTexture;
    MaskedTexture& mDownTexture;
    MaskedTexture* mActiveTexture;
    const Input& mInput;
    int mCapturedCount = 0;
};

#endif // PLAYER_H
