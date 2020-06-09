#ifndef WORLD_H
#define WORLD_H

#include <vector>

class GameObject;

/**
 * Abstract class representing the world of the game
 */
class World {
public:
    virtual ~World();

    virtual int yForLane(int lane) const = 0;

    virtual const std::vector<GameObject*>& gameObjects() const = 0;

    virtual void switchToGameOverState() = 0;
};

#endif // WORLD_H
