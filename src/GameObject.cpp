#include "GameObject.h"

#include "Mask.h"

GameObject::~GameObject() = default;

GameObject::GameObject(GameObject::Category category) : mCategory(category) {
}

bool GameObject::isActive() const {
    return mActive;
}

bool GameObject::collide(const GameObject& obj1, const GameObject& obj2) {
    const auto& rect1 = obj1.mRect;
    const auto& rect2 = obj2.mRect;
    if (!rect1.Intersects(rect2)) {
        return false;
    }
    return Mask::collide(*obj1.mMask, *obj2.mMask, {rect2.x - rect1.x, rect2.y - rect1.y});
}

void GameObject::setActive(bool active) {
    mActive = active;
}

void GameObject::setMask(const Mask* mask) {
    mMask = mask;
}
