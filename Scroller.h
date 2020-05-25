#ifndef SCROLLER_H
#define SCROLLER_H

class Scroller {
public:
    class Listener {
    public:
        virtual void spawnThings() = 0;
    };

    Scroller(Listener& listener) : mListener(listener) {
    }

    void update(float delta) {
        float oldPos = mPos;
        mPos += mScrollSpeed * delta;
        if (int(mPos) % SPAWN_SPACING < int(oldPos) % SPAWN_SPACING) {
            mListener.spawnThings();
            accelerate();
        }
    }

    float getPosition() const {
        return mPos;
    }

private:
    void accelerate() {
        mScrollSpeed *= SCROLL_SPEED_MULTIPLIER;
    }

    Listener& mListener;
    float mPos = 0;
    float mScrollSpeed = START_SCROLL_SPEED;
};

#endif /* SCROLLER_H */
