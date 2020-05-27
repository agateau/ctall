#include "GameObject.h"

GameObject::~GameObject() = default;

bool GameObject::isActive() const {
    return mActive;
}

void GameObject::setActive(bool active) {
    mActive = active;
}
