#ifndef WORLDIMPL_H
#define WORLDIMPL_H

#include "Background.h"
#include "Bonus.h"
#include "Input.h"
#include "Player.h"
#include "Pool.h"
#include "Scroller.h"
#include "Wall.h"
#include "World.h"

class Assets;

class WorldImpl : public World, public Scroller::Listener, public Background::SectionProvider {
public:
    enum class State { Running, Paused, GameOver };

    WorldImpl(Assets& assets, const Input& input);

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

    void switchToGameOverState() override;

    // Scroller::Listener
    void spawnThings() override;

    // Background::SectionProvider
    const Section* getSection() const override;

private:
    void createSections();

    Scroller mScroller;
    Background mBackground;
    Player mPlayer;
    Pool<Wall> mWallPool;
    Pool<Bonus> mBonusPool;
    Assets& mAssets;
    std::vector<GameObject*> mGameObjects;
    std::vector<Section> mSections;

    State mState = State::Running;
};

#endif // WORLDIMPL_H
