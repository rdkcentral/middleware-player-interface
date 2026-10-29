#include <SDL.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {

constexpr int kMargin = 24;
constexpr int kUrlY = 20;
constexpr int kUrlHeight = 46;
constexpr int kButtonY = 82;
constexpr int kButtonWidth = 96;
constexpr int kButtonHeight = 40;
constexpr int kButtonGap = 12;
constexpr int kVideoY = 170;

struct Button {
    SDL_Rect bounds;
    const char* label;
    SDL_Color color;
};

struct TextCache {
    std::string text;
    SDL_Color color{};
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
};

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
              bool urlFocused,
              const std::array<Button, 3>& buttons,
              TextCache& urlText,
              TextCache& statusLabel,
              TextCache& statusText,
              TextCache& videoLabel,
              TextCache& playLabel,
              TextCache& pauseLabel,
              TextCache& stopLabel)
{
    const SDL_Color background{16, 19, 25, 255};
    const SDL_Color foreground{238, 241, 245, 255};
    const SDL_Color secondary{151, 161, 175, 255};
    const SDL_Color field{25, 30, 39, 255};
    const SDL_Color border{57, 68, 84, 255};
    const SDL_Color video{7, 9, 13, 255};

    if (SDL_SetRenderDrawColor(renderer, background.r, background.g,
                               background.b, background.a) != 0 ||
        SDL_RenderClear(renderer) != 0) {
        std::cerr << "Could not clear simulator window: " << SDL_GetError() << '\n';
        return false;
    }

    const SDL_Rect urlBounds{kMargin, kUrlY, width - 2 * kMargin, kUrlHeight};
    const SDL_Rect urlInner{urlBounds.x + 2, urlBounds.y + 2,
                            urlBounds.w - 4, urlBounds.h - 4};
    if (!FillRectangle(renderer, urlBounds, border) ||
        !FillRectangle(renderer, urlInner, field)) {
        return false;
    }

    const SDL_Rect urlClip{urlInner.x + 10, urlInner.y + 4,
                           urlInner.w - 20, urlInner.h - 8};
    if (SDL_RenderSetClipRect(renderer, &urlClip) != 0) {
        std::cerr << "Could not clip URL text: " << SDL_GetError() << '\n';
        return false;
    }
    const std::string shownUrl = url.empty() ? "Enter stream URL" : url;
    if (!DrawText(renderer, font, urlText, shownUrl, urlClip.x,
                  urlClip.y + 1, url.empty() ? secondary : foreground)) {
        SDL_RenderSetClipRect(renderer, nullptr);
        return false;
    }
    if (urlFocused) {
        int textWidth = 0;
        int textHeight = 0;
        if (!url.empty() && TTF_SizeUTF8(font, url.c_str(), &textWidth, &textHeight) != 0) {
            std::cerr << "Could not measure URL text: " << TTF_GetError() << '\n';
            SDL_RenderSetClipRect(renderer, nullptr);
            return false;
        }
        const int cursorX = std::min(urlClip.x + textWidth, urlClip.x + urlClip.w - 2);
        const SDL_Rect cursor{cursorX, urlClip.y + 2, 2, urlClip.h - 4};
        if (!FillRectangle(renderer, cursor, foreground)) {
            SDL_RenderSetClipRect(renderer, nullptr);
            return false;
        }
    }
    if (SDL_RenderSetClipRect(renderer, nullptr) != 0) {
        std::cerr << "Could not reset URL text clipping: " << SDL_GetError() << '\n';
        return false;
    }

    const TextCache* buttonTexts[] = {&playLabel, &pauseLabel, &stopLabel};
    for (std::size_t index = 0; index < buttons.size(); ++index) {
        if (!FillRectangle(renderer, buttons[index].bounds, buttons[index].color) ||
            !DrawCachedText(renderer, *buttonTexts[index],
                            buttons[index].bounds.x +
                                (buttons[index].bounds.w - buttonTexts[index]->width) / 2,
                            buttons[index].bounds.y +
                                (buttons[index].bounds.h - buttonTexts[index]->height) / 2)) {
            return false;
        }
    }

    if (!DrawCachedText(renderer, statusLabel, kMargin, 140) ||
        !DrawCachedText(renderer, statusText, kMargin + statusLabel.width + 10, 140)) {
        return false;
    }

    const SDL_Rect videoBounds{kMargin, kVideoY,
                               width - 2 * kMargin,
                               std::max(80, height - kVideoY - kMargin)};
    const SDL_Rect videoInner{videoBounds.x + 2, videoBounds.y + 2,
                              videoBounds.w - 4, videoBounds.h - 4};
    if (!FillRectangle(renderer, videoBounds, border) ||
        !FillRectangle(renderer, videoInner, video) ||
        !DrawCachedText(renderer, videoLabel,
                        videoInner.x + (videoInner.w - videoLabel.width) / 2,
                        videoInner.y + (videoInner.h - videoLabel.height) / 2)) {
        return false;
    }

    SDL_RenderPresent(renderer);
    return true;
}

bool Contains(const Button& button, int x, int y)
{
    return x >= button.bounds.x &&
           x < button.bounds.x + button.bounds.w &&
           y >= button.bounds.y &&
           y < button.bounds.y + button.bounds.h;
}

void DestroyTextCaches(const std::array<TextCache*, 7>& caches)
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
    const std::array<Button, 3> buttons{{
        {{kMargin, kButtonY, kButtonWidth, kButtonHeight}, "Play", buttonColors[0]},
        {{kMargin + kButtonWidth + kButtonGap, kButtonY,
          kButtonWidth, kButtonHeight}, "Pause", buttonColors[1]},
        {{kMargin + 2 * (kButtonWidth + kButtonGap), kButtonY,
          kButtonWidth, kButtonHeight}, "Stop", buttonColors[2]}
    }};

    TextCache urlText;
    TextCache statusLabel;
    TextCache statusText;
    TextCache videoLabel;
    TextCache playLabel;
    TextCache pauseLabel;
    TextCache stopLabel;
    const std::array<TextCache*, 7> textCaches{{
        &urlText, &statusLabel, &statusText, &videoLabel,
        &playLabel, &pauseLabel, &stopLabel
    }};

    const bool textReady =
        UpdateText(renderer, font, statusLabel, "Status", secondary) &&
        UpdateText(renderer, font, videoLabel, "Video output area", secondary) &&
        UpdateText(renderer, font, playLabel, "Play", foreground) &&
        UpdateText(renderer, font, pauseLabel, "Pause", foreground) &&
        UpdateText(renderer, font, stopLabel, "Stop", foreground);
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
    std::string status = "Ready. Playback is mocked.";
    bool urlFocused = false;
    bool running = true;
    int exitCode = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN &&
                       event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {
                const int mouseX = event.button.x;
                const int mouseY = event.button.y;
                int windowWidth = 0;
                int windowHeight = 0;
                SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                urlFocused = mouseX >= kMargin &&
                             mouseX < windowWidth - kMargin &&
                             mouseY >= kUrlY &&
                             mouseY < std::min(kUrlY + kUrlHeight, windowHeight);
                if (urlFocused) {
                    SDL_StartTextInput();
                    continue;
                }

                SDL_StopTextInput();
                if (Contains(buttons[0], mouseX, mouseY)) {
                    status = url.empty() ? "Enter a URL first." : "Playing (mock).";
                } else if (Contains(buttons[1], mouseX, mouseY)) {
                    status = "Paused (mock).";
                } else if (Contains(buttons[2], mouseX, mouseY)) {
                    status = "Stopped.";
                }
            } else if (event.type == SDL_TEXTINPUT && urlFocused) {
                const std::string input(event.text.text);
                if (url.size() + input.size() <= 512) {
                    url += input;
                }
            } else if (event.type == SDL_KEYDOWN && urlFocused) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !url.empty()) {
                    std::size_t lastCharacter = url.size() - 1;
                    while (lastCharacter > 0 &&
                           (static_cast<unsigned char>(url[lastCharacter]) & 0xC0) == 0x80) {
                        --lastCharacter;
                    }
                    url.erase(lastCharacter);
                } else if (event.key.keysym.sym == SDLK_RETURN ||
                           event.key.keysym.sym == SDLK_KP_ENTER) {
                    urlFocused = false;
                    SDL_StopTextInput();
                    status = url.empty() ? "Ready. Playback is mocked."
                                         : "URL entered. Playback is mocked.";
                } else if ((event.key.keysym.mod & KMOD_CTRL) != 0 &&
                           event.key.keysym.sym == SDLK_a) {
                    url.clear();
                }
            }
        }

        int width = 0;
        int height = 0;
        SDL_GetWindowSize(window, &width, &height);
        if (!UpdateText(renderer, font, urlText,
                        url.empty() ? "Enter stream URL" : url,
                        url.empty() ? secondary : foreground) ||
            !UpdateText(renderer, font, statusText, status, foreground) ||
            !RenderUi(renderer, font, width, height, url, urlFocused,
                      buttons, urlText, statusLabel, statusText, videoLabel,
                      playLabel, pauseLabel, stopLabel)) {
            exitCode = 1;
            running = false;
        }
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
