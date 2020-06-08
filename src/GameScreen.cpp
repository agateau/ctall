#include "GameScreen.h"

#include <SDL2/SDL.h>

#include <SDL2pp/SDL2pp.hh>

#include "Assets.h"
#include "CtallApp.h"
#include "Random.h"
#include "Scroller.h"
#include "Wall.h"

static constexpr int SCORE_ROUND = 100;
static constexpr int SCORE_PER_CAPTURE = 1000;

static constexpr int SECTION_COUNT = 10;
static constexpr int MIN_SECTION_LENGTH = 4;
static constexpr int MAX_SECTION_LENGTH = 15;

using namespace SDL2pp;
using namespace Random;

GameScreen::GameScreen(CtallApp& app)
        : mApp(app)
        , mAssets(app.assets())
        , mPlayer(*this, mAssets.player, mAssets.playerUp, mAssets.playerDown, mInput)
        , mScroller(*this)
        , mBackground(*this, mScroller, *this)
        , mWallPool([this]() { return new Wall(*this, mWallPool, mScroller, mAssets.wall); })
        , mBonusPool([this]() { return new Bonus(*this, mBonusPool, mScroller, mAssets.bonuses); })
        , mGameOverMenu(mAssets.textDrawer)
        , mPauseMenu(mAssets.textDrawer) {
    mGameObjects.push_back(&mPlayer);

    mGameOverMenu.addItem("RESTART", [&app] { app.showGameScreen(); });
    mGameOverMenu.addItem("BACK TO MENU", [&app] { app.showStartScreen(); });

    mPauseMenu.addItem("CONTINUE", [this] { mState = State::Running; });
    mPauseMenu.addItem("BACK TO MENU", [&app] { app.showStartScreen(); });

    createSections();
}

GameScreen::~GameScreen() {
}

void GameScreen::spawnThings() {
    int wallLane = MIN_LANE - 1;
    if (randomBool() == 0) {
        auto wall = mWallPool.get();
        wallLane = randomRange(MIN_LANE, MAX_LANE + 1);
        wall->setup(wallLane);
        mGameObjects.push_back(wall);
    }

    if (randomRange(4) == 0) {
        auto bonus = mBonusPool.get();
        int bonusLane = wallLane;
        while (bonusLane == wallLane) {
            bonusLane = randomRange(MIN_LANE, MAX_LANE + 1);
        }
        bonus->setup(bonusLane);
        mGameObjects.push_back(bonus);
    }
}

void GameScreen::update(float delta) {
    if (mState != State::Running) {
        return;
    }
    mScroller.update(delta);
    mBackground.update();
    // Do not use an iterator here since it might become invalid if mGameObjects is resized after
    // an addition
    for (int idx = int(mGameObjects.size() - 1); idx >= 0; --idx) {
        mGameObjects.at(idx)->update(delta);
    }
    mGameObjects.erase(std::remove_if(mGameObjects.begin(),
                                      mGameObjects.end(),
                                      [](GameObject* object) { return !object->isActive(); }),
                       mGameObjects.end());
}

void GameScreen::draw(Renderer& renderer) {
    mBackground.draw(renderer);

    // Ensure game objects are drawn in the right order
    std::sort(mGameObjects.begin(), mGameObjects.end(), [](const auto* o1, const auto* o2) {
        return o1->rect().y < o2->rect().y;
    });

    for (auto* object : mGameObjects) {
        object->draw(renderer);
    }
    drawHud(renderer);
    if (mState == State::GameOver) {
        drawGameOverOverlay(renderer);
    } else if (mState == State::Paused) {
        drawPauseOverlay(renderer);
    }
}

void GameScreen::onEvent(const SDL_Event& event) {
    if (mState == State::GameOver) {
        mGameOverMenu.onEvent(event);
    } else if (mState == State::Paused) {
        mPauseMenu.onEvent(event);
    }
}

void GameScreen::drawHud(Renderer& renderer) {
    static char scoreText[40];
    std::snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score());
    mAssets.textDrawer.draw(renderer, scoreText, {0, 0});
}

int GameScreen::score() const {
    int distance = int(mScroller.getPosition() / SCORE_ROUND) * SCORE_ROUND;
    return distance + mPlayer.capturedCount() * SCORE_PER_CAPTURE;
}

void GameScreen::drawGameOverOverlay(Renderer& renderer) {
    Point center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    mAssets.textDrawer.draw(
        renderer, "GAME OVER\n\n", center, TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mGameOverMenu.draw(renderer, center);
}

void GameScreen::drawPauseOverlay(Renderer& renderer) {
    Point center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    mAssets.textDrawer.draw(
        renderer, "PAUSED\n\n", center, TextDrawer::HCENTER | TextDrawer::BOTTOM);
    mPauseMenu.draw(renderer, center);
}

void GameScreen::onKeyPressed(const SDL_KeyboardEvent& event) {
    if (event.keysym.sym == SDLK_UP) {
        mInput.up = true;
    } else if (event.keysym.sym == SDLK_DOWN) {
        mInput.down = true;
    }
}

void GameScreen::onKeyReleased(const SDL_KeyboardEvent& event) {
    auto sym = event.keysym.sym;
    if (mState == State::GameOver) {
        if (sym == SDLK_SPACE) {
            mApp.showGameScreen();
        }
        return;
    }
    if (sym == SDLK_UP) {
        mInput.up = false;
    } else if (sym == SDLK_DOWN) {
        mInput.down = false;
    } else if (sym == SDLK_ESCAPE) {
        mState = State::Paused;
    }
}

int GameScreen::yForLane(int lane) const {
    return SCREEN_HEIGHT / 2 + lane * LANE_WIDTH - LANE_WIDTH / 2;
}

const std::vector<GameObject*>& GameScreen::activeGameObjects() const {
    return mGameObjects;
}

void GameScreen::switchToGameOverState() {
    mState = State::GameOver;
}

const Section* GameScreen::getSection() const {
    return &randomChoice(mSections);
}

/**
 * At one point sections will be created from assets
 */
void GameScreen::createSections() {
    for (int i = 0; i < SECTION_COUNT; ++i) {
        std::list<Section::Column> columns;
        int length = randomRange(MIN_SECTION_LENGTH, MAX_SECTION_LENGTH);
        const auto& assets = randomChoice(mAssets.backgrounds);
        for (int columnIdx = 0; columnIdx < length; ++columnIdx) {
            Section::Column column(MAX_LANE - MIN_LANE + 1 + 2); // + 2 for borders
            auto it = column.begin();
            auto last = column.end() - 1;
            *it = &assets.border;
            ++it;
            for (; it != last; ++it) {
                *it = &Random::randomChoice(assets.roads);
            }
            *last = &assets.border;
            columns.push_back(column);
        }
        mSections.emplace_back(Section{columns});
    }
}
