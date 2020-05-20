#ifndef WALL_H
#define WALL_H

#include "GameObject.h"
#include "ScrollComponent.h"

class Wall : public GameObject {
public:
    Wall(Pool<Wall>& pool, const Scroller& scroller, const sf::Texture& texture)
        : mPool(pool), mScrollComponent(scroller, mSprite)
    {
        mSprite.setTexture(texture);
    }

    void setup(int row) {
        mSprite.setPosition({SCREEN_WIDTH, SCREEN_HEIGHT / 2 + row * ROW_HEIGHT});
        mScrollComponent.setup();
    }

    void update(sf::Time delta) override {
        mScrollComponent.update();
        if (mSprite.getPosition().x + ROW_HEIGHT < 0) {
            mPool.recycle(this);
        }
    }

    void draw(sf::RenderTarget& target) override {
        target.draw(mSprite);
    }

private:
    sf::Sprite mSprite;
    Pool<Wall>& mPool;
    ScrollComponent mScrollComponent;
};

#endif /* WALL_H */
