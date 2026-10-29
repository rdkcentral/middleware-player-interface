#include <SDL.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr int kOverlayMargin = 16;
constexpr int kOverlayHeight = 56;
constexpr int kOverlayInset = 12;
constexpr int kButtonHeight = 40;
constexpr Uint32 kControlsHideDelayMs = 3000;

struct Button {
    SDL_Rect bounds;
    const char* label;
    SDL_Color color;
};

struct Layout {
    SDL_Rect topPanel;
    SDL_Rect openButton;
    SDL_Rect urlField;
    SDL_Rect urlClip;
    SDL_Rect loadButton;
    SDL_Rect streamClip;
    SDL_Rect controlsPanel;
    SDL_Rect statusClip;
    std::array<Button, 3> playbackButtons;
};

struct TextCache {
    std::string text;
    SDL_Color color{};
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};

Layout CreateLayout(int width, int height)
{
    const SDL_Color playColor{35, 122, 89, 255};
    const SDL_Color pauseColor{159, 112, 43, 255};
    const SDL_Color stopColor{158, 61, 64, 255};
    const int panelWidth = std::max(0, width - 2 * kOverlayMargin);
    const SDL_Rect topPanel{kOverlayMargin, kOverlayMargin,
                            panelWidth, kOverlayHeight};
    const int controlY = std::max(kOverlayMargin,
                                  height - kOverlayMargin - kOverlayHeight);
    const SDL_Rect controlsPanel{kOverlayMargin, controlY,
                                 panelWidth, kOverlayHeight};

    const SDL_Rect openButton{
        topPanel.x + kOverlayInset,
        topPanel.y + (kOverlayHeight - kButtonHeight) / 2,
        132,
        kButtonHeight
    };
    const SDL_Rect loadButton{
        topPanel.x + topPanel.w - kOverlayInset - 76,
        openButton.y,
        76,
        kButtonHeight
    };
    const SDL_Rect urlField{
        openButton.x,
        openButton.y,
        std::max(80, loadButton.x - openButton.x - kOverlayInset),
        kButtonHeight
    };
    const SDL_Rect urlClip{
        urlField.x + 10,
        urlField.y + 5,
        std::max(0, urlField.w - 20),
        urlField.h - 10
    };
    const int streamTextX = openButton.x + openButton.w + 16;
    const SDL_Rect streamClip{
        streamTextX,
        topPanel.y + 10,
        std::max(0, topPanel.x + topPanel.w - kOverlayInset - streamTextX),
        topPanel.h - 20
    };

    const int buttonY = controlsPanel.y + (kOverlayHeight - kButtonHeight) / 2;
    const SDL_Rect playBounds{
        controlsPanel.x + kOverlayInset, buttonY, 76, kButtonHeight
    };
    const SDL_Rect pauseBounds{
        playBounds.x + playBounds.w + 8, buttonY, 82, kButtonHeight
    };
    const SDL_Rect stopBounds{
        pauseBounds.x + pauseBounds.w + 8, buttonY, 72, kButtonHeight
    };
    const int statusX = stopBounds.x + stopBounds.w + 20;
    const SDL_Rect statusClip{
        statusX,
        controlsPanel.y + 10,
        std::max(0, controlsPanel.x + controlsPanel.w - kOverlayInset - statusX),
        controlsPanel.h - 20
    };

    return {
        topPanel,
        openButton,
        urlField,
        urlClip,
        loadButton,
        streamClip,
        controlsPanel,
        statusClip,
        {{
            {playBounds, "Play", playColor},
            {pauseBounds, "Pause", pauseColor},
            {stopBounds, "Stop", stopColor}
        }}
    };
}

bool SameColor(SDL_Color left, SDL_Color right)
{
    return left.r == right.r && left.g == right.g &&
           left.b == right.b && left.a == right.a;
}

bool UpdateText(SDL_Renderer* renderer,
                TTF_Font* font,
                TextCache& cache,
                const std::string& text,
                SDL_Color color)
{
    if (cache.texture != nullptr && cache.text == text &&
        SameColor(cache.color, color)) {
        return true;
    }

    if (cache.texture != nullptr) {
        SDL_DestroyTexture(cache.texture);
        cache.texture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (surface == nullptr) {
        std::cerr << "Could not render UI text: " << TTF_GetError() << '\n';
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        std::cerr << "Could not create UI text texture: " << SDL_GetError() << '\n';
        SDL_FreeSurface(surface);
        return false;
    }

    cache.width = surface->w;
    cache.height = surface->h;
    SDL_FreeSurface(surface);
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
        std::cerr << "Could not enable text transparency: " << SDL_GetError() << '\n';
        SDL_DestroyTexture(texture);
        return false;
    }

    cache.text = text;
    cache.color = color;
    cache.texture = texture;
    return true;
}

bool DrawCachedText(SDL_Renderer* renderer,
                    const TextCache& text,
                    int x,
                    int y)
{
    if (text.texture == nullptr) {
        return true;
    }
    const SDL_Rect destination{x, y, text.width, text.height};
    if (SDL_RenderCopy(renderer, text.texture, nullptr, &destination) != 0) {
        std::cerr << "Could not draw UI text: " << SDL_GetError() << '\n';
        return false;
    }
    return true;
}

bool DrawText(SDL_Renderer* renderer,
              TTF_Font* font,
              TextCache& cache,
              const std::string& value,
              int x,
              int y,
              SDL_Color color)
{
    return UpdateText(renderer, font, cache, value, color) &&
           DrawCachedText(renderer, cache, x, y);
}

bool FitTextToWidth(TTF_Font* font,
                    const std::string& text,
                    int maxWidth,
                    std::string& fitted)
{
    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeUTF8(font, text.c_str(), &textWidth, &textHeight) != 0) {
        std::cerr << "Could not measure stream URL: " << TTF_GetError() << '\n';
        return false;
    }
    if (textWidth <= maxWidth) {
        fitted = text;
        return true;
    }

    const std::string ellipsis = "...";
    int ellipsisWidth = 0;
    if (TTF_SizeUTF8(font, ellipsis.c_str(), &ellipsisWidth, &textHeight) != 0) {
        std::cerr << "Could not measure URL truncation marker: "
                  << TTF_GetError() << '\n';
        return false;
    }
    if (ellipsisWidth > maxWidth) {
        fitted.clear();
        return true;
    }

    std::vector<std::size_t> boundaries{0};
    for (std::size_t offset = 0; offset < text.size();) {
        const unsigned char firstByte = static_cast<unsigned char>(text[offset]);
        std::size_t characterBytes = 1;
        if ((firstByte & 0xE0) == 0xC0) {
            characterBytes = 2;
        } else if ((firstByte & 0xF0) == 0xE0) {
            characterBytes = 3;
        } else if ((firstByte & 0xF8) == 0xF0) {
            characterBytes = 4;
        }
        offset = std::min(offset + characterBytes, text.size());
        boundaries.push_back(offset);
    }

    std::size_t low = 0;
    std::size_t high = boundaries.size() - 1;
    while (low < high) {
        const std::size_t middle = (low + high + 1) / 2;
        const std::string candidate = text.substr(0, boundaries[middle]) + ellipsis;
        if (TTF_SizeUTF8(font, candidate.c_str(), &textWidth, &textHeight) != 0) {
            std::cerr << "Could not measure shortened stream URL: "
                      << TTF_GetError() << '\n';
            return false;
        }
        if (textWidth <= maxWidth) {
            low = middle;
        } else {
            high = middle - 1;
        }
    }
    fitted = text.substr(0, boundaries[low]) + ellipsis;
    return true;
}

bool FillRectangle(SDL_Renderer* renderer,
                   const SDL_Rect& rectangle,
                   SDL_Color color)
{
    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 ||
        SDL_RenderFillRect(renderer, &rectangle) != 0) {
        std::cerr << "Could not draw UI rectangle: " << SDL_GetError() << '\n';
        return false;
    }
    return true;
}

bool RenderUi(SDL_Renderer* renderer,
              TTF_Font* font,
              int width,
              int height,
              const std::string& url,
              const std::string& urlDraft,
              bool editingUrl,
              bool controlsVisible,
              const Layout& layout,
              TextCache& urlText,
              TextCache& currentStreamText,
              TextCache& statusText,
              TextCache& videoLabel,
              TextCache& playLabel,
              TextCache& pauseLabel,
              TextCache& stopLabel,
              TextCache& openLabel,
              TextCache& loadLabel,
              TextCache& shortcutLabel,
              TextCache& controlsHint)
{
    const SDL_Color background{7, 9, 13, 255};
    const SDL_Color foreground{238, 241, 245, 255};
    const SDL_Color secondary{151, 161, 175, 255};
    const SDL_Color field{25, 30, 39, 255};
    const SDL_Color border{57, 68, 84, 255};
    const SDL_Color overlay{10, 12, 16, 220};
    const SDL_Color openColor{43, 88, 130, 255};
    const SDL_Color video{7, 9, 13, 255};

    if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0 ||
        SDL_SetRenderDrawColor(renderer, background.r, background.g,
                               background.b, background.a) != 0 ||
        SDL_RenderClear(renderer) != 0) {
        std::cerr << "Could not clear simulator window: " << SDL_GetError() << '\n';
        return false;
    }

    const SDL_Rect videoBounds{0, 0, width, height};
    if (!FillRectangle(renderer, videoBounds, video) ||
        !UpdateText(renderer, font, videoLabel,
                    url.empty() ? "Open a stream to begin"
                                : "Video will appear here",
                    secondary)) {
        return false;
    }
    const int videoTextX = (width - videoLabel.width) / 2;
    const int videoTextY = (height - videoLabel.height) / 2;
    if (!DrawCachedText(renderer, videoLabel, videoTextX, videoTextY)) {
        return false;
    }

    if (!FillRectangle(renderer, layout.topPanel, overlay)) {
        return false;
    }
    if (editingUrl) {
        const SDL_Rect fieldInner{
            layout.urlField.x + 1,
            layout.urlField.y + 1,
            layout.urlField.w - 2,
            layout.urlField.h - 2
        };
        if (!FillRectangle(renderer, layout.urlField, border) ||
            !FillRectangle(renderer, fieldInner, field)) {
            return false;
        }

        if (SDL_RenderSetClipRect(renderer, &layout.urlClip) != 0) {
            std::cerr << "Could not clip URL text: " << SDL_GetError() << '\n';
            return false;
        }
        const std::string editorText = urlDraft.empty()
            ? "Paste stream URL"
            : urlDraft;
        if (!DrawText(renderer, font, urlText, editorText,
                      layout.urlClip.x, layout.urlClip.y,
                      urlDraft.empty() ? secondary : foreground)) {
            SDL_RenderSetClipRect(renderer, nullptr);
            return false;
        }

        int textWidth = 0;
        int textHeight = 0;
        if (!urlDraft.empty() &&
            TTF_SizeUTF8(font, urlDraft.c_str(), &textWidth, &textHeight) != 0) {
            std::cerr << "Could not measure URL text: " << TTF_GetError() << '\n';
            SDL_RenderSetClipRect(renderer, nullptr);
            return false;
        }
        const int cursorX = std::min(layout.urlClip.x + textWidth,
                                     layout.urlClip.x + layout.urlClip.w - 2);
        const SDL_Rect cursor{cursorX, layout.urlClip.y + 2, 2,
                              layout.urlClip.h - 4};
        if (!FillRectangle(renderer, cursor, foreground) ||
            SDL_RenderSetClipRect(renderer, nullptr) != 0) {
            std::cerr << "Could not draw URL cursor: " << SDL_GetError() << '\n';
            return false;
        }

        if (!FillRectangle(renderer, layout.loadButton, openColor) ||
            !DrawCachedText(renderer, loadLabel,
                            layout.loadButton.x +
                                (layout.loadButton.w - loadLabel.width) / 2,
                            layout.loadButton.y +
                                (layout.loadButton.h - loadLabel.height) / 2)) {
            return false;
        }
    } else {
        if (!FillRectangle(renderer, layout.openButton, openColor) ||
            !DrawCachedText(renderer, openLabel,
                            layout.openButton.x +
                                (layout.openButton.w - openLabel.width) / 2,
                            layout.openButton.y +
                                (layout.openButton.h - openLabel.height) / 2)) {
            return false;
        }

        if (SDL_RenderSetClipRect(renderer, &layout.streamClip) != 0) {
            std::cerr << "Could not clip current stream text: "
                      << SDL_GetError() << '\n';
            return false;
        }
        if (!DrawCachedText(renderer, shortcutLabel,
                            layout.streamClip.x, layout.streamClip.y) ||
            !DrawCachedText(renderer, currentStreamText,
                            layout.streamClip.x + shortcutLabel.width + 12,
                            layout.streamClip.y)) {
            SDL_RenderSetClipRect(renderer, nullptr);
            return false;
        }
        if (SDL_RenderSetClipRect(renderer, nullptr) != 0) {
            std::cerr << "Could not reset stream text clipping: "
                      << SDL_GetError() << '\n';
            return false;
        }
    }

    if (controlsVisible) {
        if (!FillRectangle(renderer, layout.controlsPanel, overlay)) {
            return false;
        }
        const TextCache* buttonLabels[] = {&playLabel, &pauseLabel, &stopLabel};
        for (std::size_t index = 0; index < layout.playbackButtons.size(); ++index) {
            const Button& button = layout.playbackButtons[index];
            const TextCache& label = *buttonLabels[index];
            if (!FillRectangle(renderer, button.bounds, button.color) ||
                !DrawCachedText(renderer, label,
                                button.bounds.x + (button.bounds.w - label.width) / 2,
                                button.bounds.y + (button.bounds.h - label.height) / 2)) {
                return false;
            }
        }

        if (SDL_RenderSetClipRect(renderer, &layout.statusClip) != 0) {
            std::cerr << "Could not clip status text: " << SDL_GetError() << '\n';
            return false;
        }
        if (!DrawCachedText(renderer, statusText,
                            layout.statusClip.x, layout.statusClip.y) ||
            SDL_RenderSetClipRect(renderer, nullptr) != 0) {
            std::cerr << "Could not draw status text: " << SDL_GetError() << '\n';
            return false;
        }
    } else {
        const int hintX = width - controlsHint.width - 32;
        const int hintY = height - controlsHint.height - 24;
        const SDL_Rect hintPanel{hintX - 8, hintY - 4,
                                 controlsHint.width + 16,
                                 controlsHint.height + 8};
        if (!FillRectangle(renderer, hintPanel, overlay) ||
            !DrawCachedText(renderer, controlsHint, hintX, hintY)) {
            return false;
        }
    }

    SDL_RenderPresent(renderer);
    return true;
}

bool Contains(const SDL_Rect& rectangle, int x, int y)
{
    return x >= rectangle.x && x < rectangle.x + rectangle.w &&
           y >= rectangle.y && y < rectangle.y + rectangle.h;
}

void DestroyTextCaches(const std::array<TextCache*, 11>& caches)
{
    for (TextCache* cache : caches) {
        if (cache->texture != nullptr) {
            SDL_DestroyTexture(cache->texture);
            cache->texture = nullptr;
        }
    }
}

TTF_Font* OpenSystemFont()
{
    const char* fontPaths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
    };
    for (const char* path : fontPaths) {
        TTF_Font* font = TTF_OpenFont(path, 18);
        if (font != nullptr) {
            return font;
        }
    }
    std::cerr << "Could not open a system font. Install fonts-dejavu-core. "
              << TTF_GetError() << '\n';
    return nullptr;
}

} // namespace

int main()
{
    SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << '\n';
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Could not initialize SDL_ttf: " << TTF_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Player Interface Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        960,
        600,
        SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "Could not create simulator window: " << SDL_GetError() << '\n';
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowMinimumSize(window, 480, 320);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Could not create the OpenGL ES renderer: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_RendererInfo rendererInfo{};
    if (SDL_GetRendererInfo(renderer, &rendererInfo) != 0 ||
        rendererInfo.name == nullptr ||
        std::string(rendererInfo.name) != "opengles2") {
        std::cerr << "Expected the SDL OpenGL ES renderer, but got "
                  << (rendererInfo.name == nullptr ? "an unknown renderer"
                                                   : rendererInfo.name)
                  << ".\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = OpenSystemFont();
    if (font == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    const SDL_Color foreground{238, 241, 245, 255};
    const SDL_Color secondary{151, 161, 175, 255};
    const SDL_Color buttonColors[] = {
        {35, 122, 89, 255},
        {159, 112, 43, 255},
        {158, 61, 64, 255}
    };
    TextCache urlText;
    TextCache currentStreamText;
    TextCache statusText;
    TextCache videoLabel;
    TextCache playLabel;
    TextCache pauseLabel;
    TextCache stopLabel;
    TextCache openLabel;
    TextCache loadLabel;
    TextCache shortcutLabel;
    TextCache controlsHint;
    const std::array<TextCache*, 11> textCaches{{
        &urlText, &currentStreamText, &statusText, &videoLabel,
        &playLabel, &pauseLabel, &stopLabel, &openLabel, &loadLabel,
        &shortcutLabel, &controlsHint
    }};

    const bool textReady =
        UpdateText(renderer, font, playLabel, "Play", foreground) &&
        UpdateText(renderer, font, pauseLabel, "Pause", foreground) &&
        UpdateText(renderer, font, stopLabel, "Stop", foreground) &&
        UpdateText(renderer, font, openLabel, "Open stream", foreground) &&
        UpdateText(renderer, font, loadLabel, "Load", foreground) &&
        UpdateText(renderer, font, shortcutLabel, "Ctrl+L", secondary) &&
        UpdateText(renderer, font, controlsHint,
                   "Move mouse or press Space", secondary);
    if (!textReady) {
        DestroyTextCaches(textCaches);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    std::string url;
    std::string urlDraft;
    std::string status = "Ready";
    bool editingUrl = false;
    bool mockPlaying = false;
    bool controlsVisible = true;
    bool running = true;
    int exitCode = 0;
    Uint32 lastPointerMotion = SDL_GetTicks();

    auto cancelUrlEdit = [&]() {
        editingUrl = false;
        urlDraft = url;
        SDL_StopTextInput();
    };
    auto commitUrlEdit = [&]() {
        if (urlDraft.empty()) {
            status = "Enter a URL first";
            return false;
        }
        url = urlDraft;
        editingUrl = false;
        mockPlaying = false;
        status = "Stream selected";
        SDL_StopTextInput();
        return true;
    };

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEMOTION) {
                controlsVisible = true;
                lastPointerMotion = SDL_GetTicks();
            } else if (event.type == SDL_KEYDOWN &&
                       event.key.keysym.sym == SDLK_ESCAPE) {
                if (editingUrl) {
                    cancelUrlEdit();
                } else {
                    running = false;
                }
            } else if (event.type == SDL_KEYDOWN &&
                       (event.key.keysym.mod & KMOD_CTRL) != 0 &&
                       event.key.keysym.sym == SDLK_l) {
                if (!editingUrl) {
                    editingUrl = true;
                    urlDraft = url;
                    controlsVisible = true;
                    SDL_StartTextInput();
                }
            } else if (event.type == SDL_KEYDOWN && !editingUrl) {
                controlsVisible = true;
                lastPointerMotion = SDL_GetTicks();
                if (event.key.keysym.sym == SDLK_SPACE) {
                    if (url.empty()) {
                        status = "Open a stream first";
                    } else {
                        mockPlaying = !mockPlaying;
                        status = mockPlaying ? "Playing (mock)" : "Paused";
                    }
                } else if (event.key.keysym.sym == SDLK_s) {
                    mockPlaying = false;
                    status = "Stopped";
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {
                controlsVisible = true;
                lastPointerMotion = SDL_GetTicks();
                int windowWidth = 0;
                int windowHeight = 0;
                SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                const Layout layout = CreateLayout(windowWidth, windowHeight);
                const int mouseX = event.button.x;
                const int mouseY = event.button.y;

                if (editingUrl && Contains(layout.loadButton, mouseX, mouseY)) {
                    commitUrlEdit();
                } else if (editingUrl && Contains(layout.urlField, mouseX, mouseY)) {
                    SDL_StartTextInput();
                } else if (!editingUrl &&
                           Contains(layout.openButton, mouseX, mouseY)) {
                    editingUrl = true;
                    urlDraft = url;
                    status = "Enter URL, then load";
                    SDL_StartTextInput();
                } else if (Contains(layout.playbackButtons[0].bounds, mouseX, mouseY)) {
                    if (editingUrl && !commitUrlEdit()) {
                        continue;
                    }
                    if (url.empty()) {
                        status = "Open a stream first";
                    } else {
                        mockPlaying = true;
                        status = "Playing (mock)";
                    }
                } else if (Contains(layout.playbackButtons[1].bounds, mouseX, mouseY)) {
                    cancelUrlEdit();
                    mockPlaying = false;
                    status = "Paused";
                } else if (Contains(layout.playbackButtons[2].bounds, mouseX, mouseY)) {
                    cancelUrlEdit();
                    mockPlaying = false;
                    status = "Stopped";
                } else if (editingUrl) {
                    cancelUrlEdit();
                }
            } else if (event.type == SDL_TEXTINPUT && editingUrl) {
                const std::string input(event.text.text);
                if (urlDraft.size() + input.size() <= 512) {
                    urlDraft += input;
                }
            } else if (event.type == SDL_KEYDOWN && editingUrl) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !urlDraft.empty()) {
                    std::size_t lastCharacter = urlDraft.size() - 1;
                    while (lastCharacter > 0 &&
                           (static_cast<unsigned char>(urlDraft[lastCharacter]) & 0xC0) == 0x80) {
                        --lastCharacter;
                    }
                    urlDraft.erase(lastCharacter);
                } else if (event.key.keysym.sym == SDLK_RETURN ||
                           event.key.keysym.sym == SDLK_KP_ENTER) {
                    commitUrlEdit();
                } else if ((event.key.keysym.mod & KMOD_CTRL) != 0 &&
                           event.key.keysym.sym == SDLK_a) {
                    urlDraft.clear();
                }
            }
        }

        if (mockPlaying && controlsVisible && !editingUrl &&
            SDL_GetTicks() - lastPointerMotion >= kControlsHideDelayMs) {
            controlsVisible = false;
        }

        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window, &width, &height);
        const Layout layout = CreateLayout(width, height);
        const std::string fullUrl = url.empty() ? "No stream selected" : url;
        std::string shownUrl;
        if (!FitTextToWidth(font, fullUrl,
                            layout.streamClip.w - shortcutLabel.width - 12,
                            shownUrl)) {
            exitCode = 1;
            running = false;
            continue;
        }
        const std::string shownVideo = url.empty()
            ? "Open a stream to begin"
            : "Video will appear here";
        const std::string editorText = urlDraft.empty()
            ? "Paste stream URL"
            : urlDraft;
        if (!UpdateText(renderer, font, currentStreamText, shownUrl, foreground) ||
            !UpdateText(renderer, font, statusText, status, foreground) ||
            !UpdateText(renderer, font, videoLabel, shownVideo, secondary) ||
            !UpdateText(renderer, font, urlText, editorText,
                        urlDraft.empty() ? secondary : foreground) ||
            !RenderUi(renderer, font, width, height, url, urlDraft,
                      editingUrl, controlsVisible, layout, urlText,
                      currentStreamText, statusText, videoLabel, playLabel,
                      pauseLabel, stopLabel, openLabel, loadLabel, shortcutLabel,
                      controlsHint)) {
            exitCode = 1;
            running = false;
        }
        SDL_Delay(16);
    }

    SDL_StopTextInput();
    DestroyTextCaches(textCaches);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return exitCode;
}
