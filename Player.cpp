#include "Player.h"

#include "constants.h"
#include "Input.h"

using namespace SDL2pp;

static constexpr float PX_PER_SEC = 90;

Player::Player(Texture& texture, const Input& input)
    : mTexture(texture), mInput(input), mPos{12, SCREEN_HEIGHT / 2} {
}

void Player::update(float delta) {
    if (mInput.up) {
        mPos.y -= PX_PER_SEC * delta;
    }
    if (mInput.down) {
        mPos.y += PX_PER_SEC * delta;
    }
}

void Player::draw(Renderer& renderer) {
    renderer.Copy(mTexture, NullOpt, mPos);
}
