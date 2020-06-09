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
#include "WorldImpl.h"

class Background;
class CtallApp;
class Assets;

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
