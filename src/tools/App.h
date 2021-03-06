#ifndef APP_H
#define APP_H

#include <SDL2pp/SDL2pp.hh>

// std
#include <memory>

class Screen;

class App {
public:
    App(const std::string& title, int width, int height, int scale);
    ~App();

    void setScreen(std::unique_ptr<Screen> screen);

    void run();

    void quit();

    SDL2pp::Renderer& renderer() {
        return mRenderer;
    }

    const SDL2pp::Point& screenSize() const {
        return mScreenSize;
    }

private:
    void loopStep(float delta);

    void vsyncRun();
    void noVsyncRun();

    SDL2pp::Point mScreenSize;
    const int mScale;
    SDL2pp::Window mWindow;
    SDL2pp::Renderer mRenderer;
    std::unique_ptr<Screen> mScreen;
    bool mRunning = true;
};

#endif // APP_H
