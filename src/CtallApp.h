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

private:
    Assets mAssets;
};

#endif // CTALLAPP_H
