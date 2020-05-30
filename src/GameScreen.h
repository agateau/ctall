#ifndef GAME_H
#define GAME_H

#include <SDL2pp/SDL2pp.hh>
#include <vector>

#include "Bonus.h"
#include "Input.h"
#include "Menu.h"
#include "Player.h"
#include "Pool.h"
#include "Screen.h"
#include "Scroller.h"
#include "Wall.h"

class CtallApp;
class Assets;

class GameScreen : public Screen, public Scroller::Listener {
public:
    GameScreen(CtallApp& app);

    ~GameScreen();

    void spawnThings() override;

    void update(float delta);

    void draw(SDL2pp::Renderer& renderer);

    void onEvent(const SDL_Event& event) override;

    void onKeyPressed(const SDL_KeyboardEvent& event) override;

    void onKeyReleased(const SDL_KeyboardEvent& event) override;

    int yForLane(int lane) const;

    const std::vector<GameObject*>& activeGameObjects() const;

    void switchToGameOverState();

private:
    void drawGameOverOverlay(SDL2pp::Renderer& renderer);
    void drawHud(SDL2pp::Renderer& renderer);
    int score() const;

    enum class State { Running, GameOver };

    CtallApp& mApp;
    Assets& mAssets;
    Input mInput;
    Player mPlayer;
    Scroller mScroller;
    Pool<Wall> mWallPool;
    Pool<Bonus> mBonusPool;
    Menu mGameOverMenu;
    std::vector<GameObject*> mGameObjects;
    State mState = State::Running;
};

#endif // GAME_H
