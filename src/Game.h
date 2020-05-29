#ifndef GAME_H
#define GAME_H

#include <SDL2pp/SDL2pp.hh>
#include <vector>

#include "Assets.h"
#include "Bonus.h"
#include "Input.h"
#include "Player.h"
#include "Pool.h"
#include "Screen.h"
#include "Scroller.h"
#include "Wall.h"

struct SDL_KeyboardEvent;

class Game : public Screen, public Scroller::Listener {
public:
    Game(SDL2pp::Renderer& renderer);

    ~Game();

    void spawnThings() override;

    void update(float delta);

    void draw(SDL2pp::Renderer& renderer);

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

    Assets mAssets;
    Input mInput;
    Player mPlayer;
    Scroller mScroller;
    Pool<Wall> mWallPool;
    Pool<Bonus> mBonusPool;
    std::vector<GameObject*> mGameObjects;
    State mState = State::Running;
};

#endif // GAME_H
