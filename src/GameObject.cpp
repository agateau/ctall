#include "GameObject.h"

GameObject::~GameObject() = default;

GameObject::GameObject(GameObject::Category category) : mCategory(category) {
}

bool GameObject::isActive() const {
    return mActive;
}

bool GameObject::collide(const GameObject& obj1, const GameObject& obj2) {
    return obj1.rect().Intersects(obj2.rect());
}

void GameObject::setActive(bool active) {
    mActive = active;
}
