#include "Assets.h"
#include "constants.h"
#include "GameObject.h"
#include "ScrollComponent.h"
#include "Scroller.h"
#include "Pool.h"
#include "Wall.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

static int randint(int min, int max) {
    int random = std::rand();
    return min + random % (max - min + 1);
}

static int FPS = 60;

static float PX_PER_SEC = 90;

class Player : public GameObject {
public:
    Player(const sf::Texture& texture) {
        mSprite.setTexture(texture);
        mSprite.setPosition(12, SCREEN_HEIGHT / 2);
    }

    void update(sf::Time delta) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            mSprite.move(0, -PX_PER_SEC * delta.asSeconds());
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            mSprite.move(0, PX_PER_SEC * delta.asSeconds());
        }
    }

    void draw(sf::RenderTarget& target) override {
        target.draw(mSprite);
    }

private:
    sf::Sprite mSprite;
};

class Game : public Scroller::Listener {
public:
    Game()
        : mPlayer(mAssets.player)
        , mScroller(*this)
        , mWallPool([this]() { return new Wall(mWallPool, mScroller, mAssets.wall); })
    {
    }

    void spawnThings() override {
        if (randint(0, 1) == 0) {
            auto wall = mWallPool.get();
            int row = randint(MIN_ROW, MAX_ROW);
            wall->setup(row);
        }
    }

    void update(sf::Time delta) {
        mPlayer.update(delta);
        mScroller.update(delta);
        for (auto* item : mWallPool.getActiveItems()) {
            item->update(delta);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.clear();
        mPlayer.draw(window);
        for (auto* item : mWallPool.getActiveItems()) {
            item->draw(window);
        }
    }

private:
    Assets mAssets;
    Player mPlayer;
    Pool<Wall> mWallPool;
    Scroller mScroller;
};

int main(int argc, char** argv) {
    bool useFramerateLimit = argc == 2 && std::string(argv[1]) == "--ufl";
    std::cout << "useFramerateLimit=" << useFramerateLimit << '\n';

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Catch Them All");

    if (useFramerateLimit) {
        window.setFramerateLimit(FPS);
    }

    Game game;

    sf::Clock clock;

    auto loopStep = [&window, &game](sf::Time delta) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        game.update(delta);
        game.draw(window);
        window.display();
    };

    if (useFramerateLimit) {
        while (window.isOpen()) {
            loopStep(clock.restart());
        }
    } else {
        sf::Time delta = sf::seconds(1.0/FPS);
        sf::Time nextTime = clock.getElapsedTime();
        while (window.isOpen()) {
            sf::Time currentTime = clock.getElapsedTime();
            if (currentTime >= nextTime) {
                nextTime += delta;
                loopStep(delta);
            } else {
                auto sleepTime = nextTime - currentTime;
                std::chrono::milliseconds ms(sleepTime.asMilliseconds());
                if (ms > std::chrono::milliseconds(0)) {
                    std::this_thread::sleep_for(ms);
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
