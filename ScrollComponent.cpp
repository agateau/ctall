#include "constants.h"
#include "ScrollComponent.h"
#include "Scroller.h"

ScrollComponent::ScrollComponent(const Scroller& scroller, SDL_Point& point)
    : mScroller(scroller)
    , mPoint(point)
{}

void ScrollComponent::setup() {
    mStartX = mScroller.getPosition() + SCREEN_WIDTH;
}

void ScrollComponent::update() {
    mPoint.x = mStartX - mScroller.getPosition();
}
