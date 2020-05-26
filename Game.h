#ifndef GAME_H
#define GAME_H

#include <SDL2pp/SDL2pp.hh>

#include "Assets.h"
#include "Input.h"
#include "Player.h"
#include "Pool.h"
#include "Scroller.h"
#include "Wall.h"

struct SDL_KeyboardEvent;

class Game : public Scroller::Listener {
public:
    Game(SDL2pp::Renderer& renderer);

    ~Game();

    void spawnThings() override;

    void update(float delta);

    void draw(SDL2pp::Renderer& renderer);

    void onKeyPressed(const SDL_KeyboardEvent& event);

    void onKeyReleased(const SDL_KeyboardEvent& event);

    int yForLane(int lane) const;

private:
    Assets mAssets;
    Input mInput;
    Player mPlayer;
    Scroller mScroller;
    Pool<Wall> mWallPool;
};

#endif // GAME_H
