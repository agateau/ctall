#include "ScrollComponent.h"

#include "GameObject.h"
#include "Scroller.h"

ScrollComponent::ScrollComponent(const Scroller& scroller, GameObject& gameObject)
        : mScroller(scroller), mGameObject(gameObject) {
}

void ScrollComponent::setup() {
    mStartX = int(mScroller.getPosition()) + mGameObject.rect().x;
}

void ScrollComponent::update() {
    mGameObject.rect().x = mStartX - int(mScroller.getPosition());
}
