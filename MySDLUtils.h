#ifndef MYSDLUTILS_H
#define MYSDLUTILS_H

#include <SDL2/SDL.h>

#include <string>

template <typename Ptr, void(*cleanup)(Ptr)>
class MySDLPointer {
public:
    MySDLPointer() = default;
    MySDLPointer(Ptr ptr) : mPtr(ptr) {}
    MySDLPointer(const MySDLPointer&) = delete;
    MySDLPointer& operator=(const MySDLPointer&) = delete;

    ~MySDLPointer() {
        if (mPtr) {
            cleanup(mPtr);
        }
    }

    bool operator!() const {
        return !mPtr;
    }

    void reset(Ptr ptr) {
        if (mPtr == ptr) {
            return;
        }
        if (mPtr) {
            cleanup(mPtr);
        }
        mPtr = ptr;
    }

    Ptr get() const {
        return mPtr;
    }

    Ptr release() {
        Ptr ptr = mPtr;
        mPtr = nullptr;
        return ptr;
    }

private:
    Ptr mPtr = nullptr;
};

using Window = MySDLPointer<SDL_Window*, SDL_DestroyWindow>;

class Renderer : public MySDLPointer<SDL_Renderer*, SDL_DestroyRenderer> {
public:
    Renderer(SDL_Renderer* renderer): MySDLPointer(renderer) {}

    void renderTexture(SDL_Texture* texture, const SDL_Rect& dst) const {
        SDL_RenderCopy(get(), texture, nullptr, &dst);
    }

    void renderTexture(SDL_Texture* texture, int x, int y) const {
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
        renderTexture(texture, dst);
    }
};

using Surface = MySDLPointer<SDL_Surface*, SDL_FreeSurface>;
using Texture = MySDLPointer<SDL_Texture*, SDL_DestroyTexture>;

SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& path);

#endif /* MYSDLUTILS_H */
