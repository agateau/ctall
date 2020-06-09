#ifndef GAME_H
#define GAME_H

#include <SDL2pp/SDL2pp.hh>
#include <vector>

#include "Background.h"
#include "Bonus.h"
#include "Input.h"
#include "Menu.h"
#include "Player.h"
#include "Pool.h"
#include "Screen.h"
#include "Scroller.h"
#include "Wall.h"
#include "World.h"

class Background;
class CtallApp;
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

class GameScreen : public Screen {
public:
    GameScreen(CtallApp& app);

    ~GameScreen();

    void update(float delta) override;

    void draw(SDL2pp::Renderer& renderer) override;

    void onEvent(const SDL_Event& event) override;

    void onKeyPressed(const SDL_KeyboardEvent& event) override;

    void onKeyReleased(const SDL_KeyboardEvent& event) override;

private:
    void drawGameOverOverlay(SDL2pp::Renderer& renderer);
    void drawPauseOverlay(SDL2pp::Renderer& renderer);
    void drawHud(SDL2pp::Renderer& renderer);

    CtallApp& mApp;
    Assets& mAssets;
    WorldImpl mWorld;
    Input mInput;
    Menu mGameOverMenu;
    Menu mPauseMenu;
};

#endif // GAME_H
