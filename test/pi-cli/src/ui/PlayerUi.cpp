#include "PlayerUi.h"

#include "../backend/PlayerBackend.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>

namespace {

constexpr int kMargin = 18;
constexpr int kTopPanelHeight = 72;
constexpr int kControlsHeight = 86;
constexpr Uint32 kControlsHideDelayMs = 3000;

struct Layout {
    SDL_Rect topPanel;
    SDL_Rect controlsPanel;
    SDL_Rect playButton;
    SDL_Rect stopButton;
    SDL_Rect progressTrack;
    SDL_Rect progressClip;
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
    const int panelWidth = std::max(0, width - 2 * kMargin);
    const int controlsY = std::max(
        kMargin, height - kMargin - kControlsHeight);
    const SDL_Rect controls{kMargin, controlsY, panelWidth, kControlsHeight};
    const SDL_Rect play{controls.x + 14, controls.y + 16, 54, 54};
    const SDL_Rect stop{play.x + play.w + 10, play.y, 54, 54};
    const int progressX = stop.x + stop.w + 24;
    const int progressY = controls.y + 35;
    const SDL_Rect progress{
        progressX,
        progressY,
        std::max(0, controls.x + controls.w - progressX - 18),
        10
    };

    return {
        {kMargin, kMargin, panelWidth, kTopPanelHeight},
        controls,
        play,
        stop,
        progress,
        {progress.x, progress.y - 10, progress.w, 30}
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

    cache.texture = SDL_CreateTextureFromSurface(renderer, surface);
    cache.width = surface->w;
    cache.height = surface->h;
    SDL_FreeSurface(surface);
    if (cache.texture == nullptr) {
        std::cerr << "Could not create UI text texture: "
                  << SDL_GetError() << '\n';
        return false;
    }

    cache.text = text;
    cache.color = color;
    return true;
}

bool DrawText(SDL_Renderer* renderer,
              TTF_Font* font,
              TextCache& cache,
              const std::string& text,
              SDL_Color color,
              int x,
              int y,
              int maxWidth)
{
    if (maxWidth <= 0) {
        return true;
    }

    std::string displayText = text;
    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeUTF8(font, displayText.c_str(), &textWidth, &textHeight) != 0) {
        std::cerr << "Could not measure UI text: " << TTF_GetError() << '\n';
        return false;
    }
    if (textWidth > maxWidth) {
        std::string shortened = displayText;
        if (shortened.size() > 3) {
            shortened.erase(shortened.size() - 3);
            shortened += "...";
        }
        displayText = shortened;
        if (TTF_SizeUTF8(font, displayText.c_str(),
                         &textWidth, &textHeight) != 0) {
            std::cerr << "Could not measure UI text: "
                      << TTF_GetError() << '\n';
            return false;
        }
        while (textWidth > maxWidth && displayText.size() > 3) {
            displayText.erase(displayText.size() - 4, 1);
            if (TTF_SizeUTF8(font, displayText.c_str(),
                             &textWidth, &textHeight) != 0) {
                std::cerr << "Could not measure UI text: "
                          << TTF_GetError() << '\n';
                return false;
            }
        }
        if (textWidth > maxWidth) {
            displayText.clear();
        }
    }

    if (!UpdateText(renderer, font, cache, displayText, color)) {
        return false;
    }
    const SDL_Rect destination{x, y, cache.width, cache.height};
    return SDL_RenderCopy(renderer, cache.texture, nullptr, &destination) == 0;
}

bool Fill(SDL_Renderer* renderer, const SDL_Rect& rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    return SDL_RenderFillRect(renderer, &rect) == 0;
}

bool DrawControls(SDL_Renderer* renderer,
                  const Layout& layout,
                  const PlaybackStatus& status)
{
    const SDL_Color buttonColor = status.stopped
        ? SDL_Color{78, 84, 94, 255}
        : SDL_Color{42, 49, 60, 255};
    const SDL_Color iconColor = status.stopped
        ? SDL_Color{132, 138, 148, 255}
        : SDL_Color{238, 241, 245, 255};
    if (!Fill(renderer, layout.playButton, buttonColor) ||
        !Fill(renderer, layout.stopButton, buttonColor)) {
        return false;
    }

    SDL_SetRenderDrawColor(renderer, iconColor.r, iconColor.g,
                           iconColor.b, iconColor.a);
    const int centerX = layout.playButton.x + layout.playButton.w / 2;
    const int centerY = layout.playButton.y + layout.playButton.h / 2;
    if (status.playing) {
        const SDL_Rect firstBar{centerX - 9, centerY - 12, 6, 24};
        const SDL_Rect secondBar{centerX + 3, centerY - 12, 6, 24};
        if (SDL_RenderFillRect(renderer, &firstBar) != 0 ||
            SDL_RenderFillRect(renderer, &secondBar) != 0) {
            return false;
        }
    } else {
        const SDL_Point triangle[] = {
            {centerX - 8, centerY - 13},
            {centerX + 12, centerY},
            {centerX - 8, centerY + 13},
            {centerX - 8, centerY - 13}
        };
        if (SDL_RenderDrawLines(renderer, triangle, 4) != 0) {
            return false;
        }
    }

    const SDL_Rect stopIcon{
        layout.stopButton.x + 17,
        layout.stopButton.y + 17,
        20,
        20
    };
    if (SDL_RenderFillRect(renderer, &stopIcon) != 0) {
        return false;
    }

    return Fill(renderer, layout.progressTrack, {62, 69, 80, 255});
}

std::string FormatTime(long long milliseconds)
{
    if (milliseconds < 0) {
        return "--:--";
    }

    const long long seconds = milliseconds / 1000;
    const long long minutes = seconds / 60;
    const long long hours = minutes / 60;
    const long long remainingMinutes = minutes % 60;
    const long long remainingSeconds = seconds % 60;
    char buffer[32]{};
    if (hours > 0) {
        std::snprintf(buffer, sizeof(buffer), "%lld:%02lld:%02lld",
                      hours, remainingMinutes, remainingSeconds);
    } else {
        std::snprintf(buffer, sizeof(buffer), "%02lld:%02lld",
                      minutes, remainingSeconds);
    }
    return buffer;
}

TTF_Font* OpenSystemFont()
{
    constexpr std::array<const char*, 3> paths{{
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
    }};
    for (const char* path : paths) {
        TTF_Font* font = TTF_OpenFont(path, 18);
        if (font != nullptr) {
            return font;
        }
    }

    std::cerr << "Could not open a supported system font: "
              << TTF_GetError() << '\n';
    return nullptr;
}

void DestroyTextCaches(const std::array<TextCache*, 5>& caches)
{
    for (TextCache* cache : caches) {
        if (cache->texture != nullptr) {
            SDL_DestroyTexture(cache->texture);
            cache->texture = nullptr;
        }
    }
}

} // namespace

int PlayerUi::Run(PlayerBackend& backend, const std::string& url)
{
    SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "1");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << '\n';
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Could not initialize SDL_ttf: "
                  << TTF_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Player Interface Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        960,
        600,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT |
            SDL_WINDOW_ALWAYS_ON_TOP);
    if (window == nullptr) {
        std::cerr << "Could not create simulator window: "
                  << SDL_GetError() << '\n';
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowMinimumSize(window, 480, 320);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Could not create the OpenGL ES renderer: "
                  << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

    TextCache titleText;
    TextCache urlText;
    TextCache statusText;
    TextCache positionText;
    TextCache durationText;
    const std::array<TextCache*, 5> caches{{
        &titleText, &urlText, &statusText, &positionText, &durationText
    }};
    const SDL_Color foreground{238, 241, 245, 255};
    const SDL_Color secondary{173, 181, 192, 255};
    bool running = true;
    bool controlsVisible = true;
    int exitCode = 0;
    Uint32 lastPointerMotion = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEMOTION) {
                controlsVisible = true;
                lastPointerMotion = SDL_GetTicks();
            } else if (event.type == SDL_KEYDOWN) {
                controlsVisible = true;
                lastPointerMotion = SDL_GetTicks();
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                } else if (event.key.keysym.sym == SDLK_SPACE) {
                    backend.TogglePlayback();
                } else if (event.key.keysym.sym == SDLK_s) {
                    backend.Stop();
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {
                controlsVisible = true;
                lastPointerMotion = SDL_GetTicks();
                int width = 0;
                int height = 0;
                SDL_GetWindowSize(window, &width, &height);
                const Layout layout = CreateLayout(width, height);
                const SDL_Point point{event.button.x, event.button.y};
                if (SDL_PointInRect(&point, &layout.playButton)) {
                    backend.TogglePlayback();
                } else if (SDL_PointInRect(&point, &layout.stopButton)) {
                    backend.Stop();
                }
            }
        }

        backend.PumpEvents();
        const PlaybackStatus status = backend.GetStatus();
        if (status.playing && controlsVisible &&
            SDL_GetTicks() - lastPointerMotion >= kControlsHideDelayMs) {
            controlsVisible = false;
        }

        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window, &width, &height);
        const Layout layout = CreateLayout(width, height);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        if (SDL_RenderClear(renderer) != 0) {
            std::cerr << "Could not draw simulator UI: "
                      << SDL_GetError() << '\n';
            exitCode = 1;
            break;
        }

        if (controlsVisible) {
            if (!Fill(renderer, layout.topPanel, {31, 37, 46, 238}) ||
                !Fill(renderer, layout.controlsPanel, {31, 37, 46, 238}) ||
                !DrawText(renderer, font, titleText,
                          "Player Interface Simulator",
                          foreground, layout.topPanel.x + 16,
                          layout.topPanel.y + 10,
                          layout.topPanel.w - 32) ||
                !DrawText(renderer, font, urlText, url, secondary,
                          layout.topPanel.x + 16, layout.topPanel.y + 40,
                          layout.topPanel.w - 32) ||
                !DrawText(renderer, font, statusText, status.message,
                          foreground, layout.controlsPanel.x + 18,
                          layout.controlsPanel.y + 8,
                          layout.controlsPanel.w - 36) ||
                !DrawControls(renderer, layout, status)) {
                std::cerr << "Could not render simulator UI: "
                          << SDL_GetError() << '\n';
                exitCode = 1;
                break;
            }

            const long long durationMs = status.durationMs;
            const std::string position = FormatTime(status.positionMs);
            const std::string duration = FormatTime(durationMs);
            const int timeWidth = 54;
            const int progressRight = layout.progressTrack.x +
                                     layout.progressTrack.w;
            if (!DrawText(renderer, font, positionText, position, secondary,
                          layout.progressTrack.x,
                          layout.progressTrack.y + 17, timeWidth) ||
                !DrawText(renderer, font, durationText, duration, secondary,
                          std::max(layout.progressTrack.x + timeWidth,
                                   progressRight - timeWidth),
                          layout.progressTrack.y + 17, timeWidth)) {
                std::cerr << "Could not render playback time: "
                          << SDL_GetError() << '\n';
                exitCode = 1;
                break;
            }

            if (durationMs > 0 && layout.progressTrack.w > 0) {
                const double fraction = std::clamp(
                    static_cast<double>(status.positionMs) / durationMs,
                    0.0,
                    1.0);
                SDL_Rect progress = layout.progressTrack;
                progress.w = static_cast<int>(progress.w * fraction);
                if (progress.w > 0 &&
                    !Fill(renderer, progress, {74, 160, 126, 255})) {
                    std::cerr << "Could not draw playback progress: "
                              << SDL_GetError() << '\n';
                    exitCode = 1;
                    break;
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    backend.Stop();
    DestroyTextCaches(caches);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return exitCode;
}
