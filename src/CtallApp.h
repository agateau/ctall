#ifndef CTALLAPP_H
#define CTALLAPP_H

#include "App.h"
#include "Assets.h"

class CtallApp : public App {
public:
    CtallApp();

    Assets& assets() {
        return mAssets;
    }

    void showStartScreen();
    void showGameScreen();

    int skaterIdx() const {
        return mPlayerIdx;
    }
    void setSkaterIdx(int value);

private:
    Assets mAssets;
    int mPlayerIdx = 0;
};

#endif // CTALLAPP_H
