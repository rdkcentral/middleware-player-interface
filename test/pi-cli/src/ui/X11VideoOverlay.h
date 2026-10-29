#pragma once

#include <SDL.h>

#include <memory>

class X11VideoOverlay {
public:
    explicit X11VideoOverlay(SDL_Window* window);
    ~X11VideoOverlay();

    X11VideoOverlay(const X11VideoOverlay&) = delete;
    X11VideoOverlay& operator=(const X11VideoOverlay&) = delete;

    bool Initialize();
    bool Synchronize(const SDL_Rect& topPanel,
                     const SDL_Rect& controlsPanel,
                     bool controlsVisible);
    bool PointerMovedOverVideo();
    bool IsAttached() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
