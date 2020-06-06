#ifndef APP_H
#define APP_H

#include <SDL2pp/SDL2pp.hh>

// std
#include <memory>

class Screen;

class App {
public:
    App(const std::string& title, int width, int height, int scale);

    void setScreen(std::unique_ptr<Screen> screen);

    void run();

    void quit();

    SDL2pp::Renderer& renderer() {
        return mRenderer;
    }

private:
    SDL2pp::Window mWindow;
    SDL2pp::Renderer mRenderer;
    std::unique_ptr<Screen> mScreen;
    bool mRunning = true;
};

#endif // APP_H
