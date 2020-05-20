#include "constants.h"
#include "ScrollComponent.h"
#include "Scroller.h"

ScrollComponent::ScrollComponent(const Scroller& scroller, sf::Sprite& sprite)
    : mScroller(scroller)
    , mSprite(sprite)
{}

void ScrollComponent::setup() {
    mStartX = mScroller.getPosition() + SCREEN_WIDTH;
}

void ScrollComponent::update() {
    auto pos = mSprite.getPosition();
    pos.x = mStartX - mScroller.getPosition();
    mSprite.setPosition(pos);
}
