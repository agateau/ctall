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
    void createSections();
    void fillTriggers(ColumnArray<const Trigger*>& triggers);

    Scroller mScroller;
    Background mBackground;
    Player mPlayer;
    Assets& mAssets;
    const std::unique_ptr<Trigger> mWallTrigger;
    const std::unique_ptr<Trigger> mBonusTrigger;
    std::vector<GameObject*> mGameObjects;
    std::vector<Section> mSections;

    State mState = State::Running;
};

#endif // WORLDIMPL_H
