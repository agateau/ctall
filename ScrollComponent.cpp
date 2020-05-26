#include "constants.h"
#include "ScrollComponent.h"
#include "Scroller.h"

#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

ScrollComponent::ScrollComponent(const Scroller& scroller, Point& point)
    : mScroller(scroller)
    , mPoint(point)
{}

void ScrollComponent::setup() {
    mStartX = mScroller.getPosition() + SCREEN_WIDTH;
}

void ScrollComponent::update() {
    mPoint.x = mStartX - mScroller.getPosition();
}
