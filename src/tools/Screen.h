#ifndef SCREEN_H
#define SCREEN_H

union SDL_Event;
struct SDL_KeyboardEvent;

namespace SDL2pp {
class Renderer;
}

class Screen {
public:
    ~Screen() = default;

    virtual void onEvent(const SDL_Event& event);
    virtual void onKeyPressed(const SDL_KeyboardEvent& event);
    virtual void onKeyReleased(const SDL_KeyboardEvent& event);
    virtual void update(float delta);
    virtual void draw(SDL2pp::Renderer& renderer);
};

#endif // SCREEN_H
