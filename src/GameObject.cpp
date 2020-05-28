#include "GameObject.h"

GameObject::~GameObject() = default;

GameObject::GameObject(GameObject::Category category) : mCategory(category) {
}

bool GameObject::isActive() const {
    return mActive;
}

void GameObject::setActive(bool active) {
    mActive = active;
}
