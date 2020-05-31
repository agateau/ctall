#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2pp/SDL2pp.hh>

class GameObject {
public:
    enum class Category { Player, Bad, Bonus };

    explicit GameObject(Category category);
    virtual ~GameObject();
    virtual void update(float elapsed) = 0;
    virtual void draw(SDL2pp::Renderer& renderer) = 0;

    const SDL2pp::Rect& rect() const {
        return mRect;
    }

    SDL2pp::Rect& rect() {
        return mRect;
    }

    bool isActive() const;

    Category category() const {
        return mCategory;
    }

    static bool collide(const GameObject& obj1, const GameObject& obj2);

    template <typename Iterator>
    static Iterator findCollision(const GameObject& object, Iterator first, Iterator last) {
        return std::find_if(first, last, [&object](const GameObject* other) {
            if (&object == other) {
                // Skip ourselves
                return false;
            }
            return collide(object, *other);
        });
    }

protected:
    void setActive(bool active);

    SDL2pp::Rect mRect;
    Category mCategory;

private:
    bool mActive = true;
};

#endif /* GAMEOBJECT_H */
