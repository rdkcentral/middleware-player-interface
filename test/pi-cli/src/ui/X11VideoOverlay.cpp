#include "X11VideoOverlay.h"

#if defined(PI_CLI_X11_OVERLAY)

#include <SDL_syswm.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

#include <array>
#include <string>

namespace {

constexpr const char* kVideoWindowTitle = "pi-player-simulator";
constexpr int kMaxWindowSearchDepth = 8;

bool HasTitle(Display* display, Window window, const char* expectedTitle)
{
    char* legacyTitle = nullptr;
    if (XFetchName(display, window, &legacyTitle) != 0 &&
        legacyTitle != nullptr) {
        const bool matches = std::string(legacyTitle) == expectedTitle;
        XFree(legacyTitle);
        if (matches) {
            return true;
        }
    }

    const Atom netWmName = XInternAtom(display, "_NET_WM_NAME", True);
    if (netWmName == None) {
        return false;
    }

    Atom actualType = None;
    int actualFormat = 0;
    unsigned long itemCount = 0;
    unsigned long bytesAfter = 0;
    unsigned char* value = nullptr;
    const int result = XGetWindowProperty(
        display,
        window,
        netWmName,
        0,
        256,
        False,
        AnyPropertyType,
        &actualType,
        &actualFormat,
        &itemCount,
        &bytesAfter,
        &value);
    const bool matches = result == Success && actualFormat == 8 &&
        value != nullptr &&
        std::string(reinterpret_cast<char*>(value), itemCount) ==
            expectedTitle;
    if (value != nullptr) {
        XFree(value);
    }
    return matches;
}

Window FindWindow(Display* display,
                  Window parent,
                  const char* expectedTitle,
                  int depth)
{
    if (depth > kMaxWindowSearchDepth) {
        return None;
    }

    Window root = None;
    Window parentReturn = None;
    Window* children = nullptr;
    unsigned int childCount = 0;
    if (XQueryTree(display, parent, &root, &parentReturn,
                   &children, &childCount) == 0) {
        return None;
    }

    Window match = None;
    for (unsigned int index = 0; index < childCount; ++index) {
        if (HasTitle(display, children[index], expectedTitle)) {
            match = children[index];
            break;
        }
        match = FindWindow(
            display, children[index], expectedTitle, depth + 1);
        if (match != None) {
            break;
        }
    }

    if (children != nullptr) {
        XFree(children);
    }
    return match;
}

XRectangle ToXRectangle(const SDL_Rect& rect)
{
    return {
        static_cast<short>(rect.x),
        static_cast<short>(rect.y),
        static_cast<unsigned short>(rect.w),
        static_cast<unsigned short>(rect.h)
    };
}

} // namespace

struct X11VideoOverlay::Impl {
    SDL_Window* window = nullptr;
    Display* display = nullptr;
    Window overlayWindow = None;
    Window rootWindow = None;
    Window videoWindow = None;
    int videoX = 0;
    int videoY = 0;
    int videoWidth = 0;
    int videoHeight = 0;
    int lastPointerX = 0;
    int lastPointerY = 0;
    bool hasLastPointer = false;
    bool attached = false;
};

X11VideoOverlay::X11VideoOverlay(SDL_Window* window)
    : impl_(std::make_unique<Impl>())
{
    impl_->window = window;
}

X11VideoOverlay::~X11VideoOverlay() = default;

bool X11VideoOverlay::Initialize()
{
    SDL_SysWMinfo info{};
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo(impl_->window, &info) ||
        info.subsystem != SDL_SYSWM_X11) {
        return false;
    }

    impl_->display = info.info.x11.display;
    impl_->overlayWindow = info.info.x11.window;
    impl_->rootWindow = DefaultRootWindow(impl_->display);

    int eventBase = 0;
    int errorBase = 0;
    if (!XShapeQueryExtension(impl_->display, &eventBase, &errorBase)) {
        return false;
    }
    return true;
}

bool X11VideoOverlay::Synchronize(const SDL_Rect& topPanel,
                                 const SDL_Rect& controlsPanel,
                                 bool controlsVisible)
{
    if (impl_->display == nullptr) {
        return false;
    }

    const Window videoWindow = FindWindow(
        impl_->display, impl_->rootWindow, kVideoWindowTitle, 0);
    if (videoWindow == None) {
        impl_->videoWindow = None;
        impl_->attached = false;
        return false;
    }

    XWindowAttributes attributes{};
    if (XGetWindowAttributes(impl_->display, videoWindow, &attributes) == 0) {
        impl_->videoWindow = None;
        impl_->attached = false;
        return false;
    }

    Window child = None;
    int videoX = 0;
    int videoY = 0;
    if (!XTranslateCoordinates(impl_->display, videoWindow, impl_->rootWindow,
                               0, 0, &videoX, &videoY, &child)) {
        return false;
    }

    const bool geometryChanged =
        !impl_->attached || videoX != impl_->videoX ||
        videoY != impl_->videoY ||
        attributes.width != impl_->videoWidth ||
        attributes.height != impl_->videoHeight;
    impl_->videoWindow = videoWindow;
    impl_->videoX = videoX;
    impl_->videoY = videoY;
    impl_->videoWidth = attributes.width;
    impl_->videoHeight = attributes.height;

    if (geometryChanged) {
        SDL_SetWindowBordered(impl_->window, SDL_FALSE);
        SDL_SetWindowAlwaysOnTop(impl_->window, SDL_TRUE);
        SDL_SetWindowPosition(impl_->window, videoX, videoY);
        SDL_SetWindowSize(impl_->window, attributes.width, attributes.height);
    }

    if (controlsVisible) {
        std::array<XRectangle, 2> regions{{
            ToXRectangle(topPanel),
            ToXRectangle(controlsPanel)
        }};
        XShapeCombineRectangles(impl_->display,
                                impl_->overlayWindow,
                                ShapeBounding,
                                0,
                                0,
                                regions.data(),
                                static_cast<int>(regions.size()),
                                ShapeSet,
                                YXBanded);
    } else {
        XShapeCombineRectangles(impl_->display,
                                impl_->overlayWindow,
                                ShapeBounding,
                                0,
                                0,
                                nullptr,
                                0,
                                ShapeSet,
                                YXBanded);
    }

    XRaiseWindow(impl_->display, impl_->overlayWindow);
    XFlush(impl_->display);
    impl_->attached = true;
    return true;
}

bool X11VideoOverlay::PointerMovedOverVideo()
{
    if (impl_->display == nullptr || !impl_->attached) {
        return false;
    }

    Window rootReturn = None;
    Window childReturn = None;
    int rootX = 0;
    int rootY = 0;
    int windowX = 0;
    int windowY = 0;
    unsigned int mask = 0;
    if (!XQueryPointer(impl_->display, impl_->rootWindow,
                       &rootReturn, &childReturn,
                       &rootX, &rootY, &windowX, &windowY, &mask)) {
        return false;
    }

    const bool moved = impl_->hasLastPointer &&
        (rootX != impl_->lastPointerX || rootY != impl_->lastPointerY);
    impl_->lastPointerX = rootX;
    impl_->lastPointerY = rootY;
    impl_->hasLastPointer = true;

    const bool insideVideo = rootX >= impl_->videoX &&
        rootY >= impl_->videoY &&
        rootX < impl_->videoX + impl_->videoWidth &&
        rootY < impl_->videoY + impl_->videoHeight;
    return moved && insideVideo;
}

bool X11VideoOverlay::IsAttached() const
{
    return impl_->attached;
}

#else

struct X11VideoOverlay::Impl {
};

X11VideoOverlay::X11VideoOverlay(SDL_Window*)
    : impl_(std::make_unique<Impl>())
{
}

X11VideoOverlay::~X11VideoOverlay() = default;

bool X11VideoOverlay::Initialize()
{
    return false;
}

bool X11VideoOverlay::Synchronize(const SDL_Rect&,
                                 const SDL_Rect&,
                                 bool)
{
    return false;
}

bool X11VideoOverlay::PointerMovedOverVideo()
{
    return false;
}

bool X11VideoOverlay::IsAttached() const
{
    return false;
}

#endif
