#ifndef WORLDIMPL_H
#define WORLDIMPL_H

#include "Background.h"
#include "Input.h"
#include "Player.h"
#include "Scroller.h"
#include "World.h"

// std
#include <memory>

class Assets;
class Trigger;

class WorldImpl : public World, public Background::SectionProvider {
public:
    enum class State { Running, Paused, GameOver };

    WorldImpl(Assets& assets, const Input& input);
    ~WorldImpl();

    void update(float delta);

    int score() const;

    Background& getBackground() {
        return mBackground;
    }

    State state() const;

    void switchToPauseState();

    void switchToRunningState();

    // World
    int yForLane(int lane) const override;

    const std::vector<GameObject*>& gameObjects() const override {
        return mGameObjects;
    }

    void addGameObject(GameObject* gameObject);

    void switchToGameOverState() override;

    // Background::SectionProvider
    const Section* getSection() const override;

private:
    Scroller mScroller;
    const std::unique_ptr<Trigger> mWallTrigger;
    const std::unique_ptr<Trigger> mBonusTrigger;
    TriggerMap mTriggers;
    Background mBackground;
    Player mPlayer;
    Assets& mAssets;
    std::vector<GameObject*> mGameObjects;

    State mState = State::Running;
};

#endif // WORLDIMPL_H
