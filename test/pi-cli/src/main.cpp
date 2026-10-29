#include <GLES2/gl2.h>
#include <SDL.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

std::array<GLubyte, 7> LowercaseGlyphRows(char character)
{
    switch (character) {
    case 'a': return {{0x00, 0x0E, 0x01, 0x0F, 0x11, 0x13, 0x0D}};
    case 'b': return {{0x10, 0x10, 0x1E, 0x11, 0x11, 0x11, 0x1E}};
    case 'c': return {{0x00, 0x0E, 0x11, 0x10, 0x10, 0x11, 0x0E}};
    case 'd': return {{0x01, 0x01, 0x0F, 0x11, 0x11, 0x13, 0x0D}};
    case 'e': return {{0x00, 0x0E, 0x11, 0x1F, 0x10, 0x11, 0x0E}};
    case 'f': return {{0x06, 0x09, 0x08, 0x1C, 0x08, 0x08, 0x08}};
    case 'g': return {{0x00, 0x0D, 0x13, 0x11, 0x0F, 0x01, 0x0E}};
    case 'h': return {{0x10, 0x10, 0x1E, 0x11, 0x11, 0x11, 0x11}};
    case 'i': return {{0x04, 0x00, 0x0C, 0x04, 0x04, 0x04, 0x0E}};
    case 'j': return {{0x02, 0x00, 0x06, 0x02, 0x02, 0x12, 0x0C}};
    case 'k': return {{0x10, 0x10, 0x12, 0x14, 0x18, 0x14, 0x12}};
    case 'l': return {{0x0C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E}};
    case 'm': return {{0x00, 0x1A, 0x15, 0x15, 0x15, 0x15, 0x15}};
    case 'n': return {{0x00, 0x1E, 0x11, 0x11, 0x11, 0x11, 0x11}};
    case 'o': return {{0x00, 0x0E, 0x11, 0x11, 0x11, 0x11, 0x0E}};
    case 'p': return {{0x00, 0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10}};
    case 'q': return {{0x00, 0x0F, 0x11, 0x11, 0x0F, 0x01, 0x01}};
    case 'r': return {{0x00, 0x16, 0x19, 0x10, 0x10, 0x10, 0x10}};
    case 's': return {{0x00, 0x0F, 0x10, 0x0E, 0x01, 0x01, 0x1E}};
    case 't': return {{0x08, 0x08, 0x1C, 0x08, 0x08, 0x09, 0x06}};
    case 'u': return {{0x00, 0x11, 0x11, 0x11, 0x11, 0x13, 0x0D}};
    case 'v': return {{0x00, 0x11, 0x11, 0x11, 0x0A, 0x0A, 0x04}};
    case 'w': return {{0x00, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A}};
    case 'x': return {{0x00, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x00}};
    case 'y': return {{0x00, 0x11, 0x11, 0x0F, 0x01, 0x11, 0x0E}};
    case 'z': return {{0x00, 0x1F, 0x02, 0x04, 0x08, 0x10, 0x1F}};
    default: return {};
    }
}

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

std::array<GLubyte, 7> GlyphRows(char character)
{
    if (std::islower(static_cast<unsigned char>(character)) != 0) {
        return LowercaseGlyphRows(character);
    }
    switch (character) {
    case 'A': return {{0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}};
    case 'B': return {{0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E}};
    case 'C': return {{0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E}};
    case 'D': return {{0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E}};
    case 'E': return {{0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}};
    case 'F': return {{0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10}};
    case 'G': return {{0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F}};
    case 'H': return {{0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}};
    case 'I': return {{0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E}};
    case 'J': return {{0x07, 0x02, 0x02, 0x02, 0x12, 0x12, 0x0C}};
    case 'K': return {{0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11}};
    case 'L': return {{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F}};
    case 'M': return {{0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11}};
    case 'N': return {{0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11}};
    case 'O': return {{0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}};
    case 'P': return {{0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}};
    case 'Q': return {{0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D}};
    case 'R': return {{0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}};
    case 'S': return {{0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}};
    case 'T': return {{0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}};
    case 'U': return {{0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}};
    case 'V': return {{0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04}};
    case 'W': return {{0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A}};
    case 'X': return {{0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11}};
    case 'Y': return {{0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}};
    case 'Z': return {{0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F}};
    case '0': return {{0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}};
    case '1': return {{0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}};
    case '2': return {{0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}};
    case '3': return {{0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E}};
    case '4': return {{0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}};
    case '5': return {{0x1F, 0x10, 0x10, 0x1E, 0x01, 0x01, 0x1E}};
    case '6': return {{0x0E, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x0E}};
    case '7': return {{0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}};
    case '8': return {{0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}};
    case '9': return {{0x0E, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x0E}};
    case ':': return {{0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00}};
    case '/': return {{0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x10}};
    case '.': return {{0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C}};
    case '-': return {{0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00}};
    case '_': return {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F}};
    case '?': return {{0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04}};
    case '=': return {{0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00, 0x00}};
    case '&': return {{0x0C, 0x12, 0x14, 0x08, 0x15, 0x12, 0x0D}};
    case '%': return {{0x19, 0x1A, 0x02, 0x04, 0x08, 0x0B, 0x13}};
    case '+': return {{0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00}};
    case '@': return {{0x0E, 0x11, 0x17, 0x15, 0x17, 0x10, 0x0E}};
    case '#': return {{0x0A, 0x1F, 0x0A, 0x0A, 0x1F, 0x0A, 0x00}};
    case ' ': return {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    default: return {{0x0E, 0x11, 0x01, 0x02, 0x04, 0x00, 0x04}};
    }
}

void AppendRectangle(std::vector<GLfloat>& vertices,
                     GLfloat x,
                     GLfloat y,
                     GLfloat width,
                     GLfloat height)
{
    const GLfloat rectangle[] = {
        x, y, x + width, y, x, y + height,
        x, y + height, x + width, y, x + width, y + height
    };
    vertices.insert(vertices.end(), std::begin(rectangle), std::end(rectangle));
}

class UiRenderer {
public:
    ~UiRenderer()
    {
        Shutdown();
    }

    bool Initialize()
    {
        static const char* vertexSource =
            "attribute vec2 aPosition;\n"
            "uniform vec2 uResolution;\n"
            "void main() {\n"
            "  vec2 position = vec2((aPosition.x / uResolution.x) * 2.0 - 1.0,\n"
            "                       1.0 - (aPosition.y / uResolution.y) * 2.0);\n"
            "  gl_Position = vec4(position, 0.0, 1.0);\n"
            "}\n";
        static const char* fragmentSource =
            "precision mediump float;\n"
            "uniform vec4 uColor;\n"
            "void main() { gl_FragColor = uColor; }\n";

        const GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) {
            return false;
        }
        const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (fragmentShader == 0) {
            glDeleteShader(vertexShader);
            return false;
        }

        program_ = glCreateProgram();
        if (program_ == 0) {
            std::cerr << "Could not create UI shader program.\n";
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return false;
        }
        glAttachShader(program_, vertexShader);
        glAttachShader(program_, fragmentShader);
        glLinkProgram(program_);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLint linked = GL_FALSE;
        glGetProgramiv(program_, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE) {
            PrintProgramLog(program_);
            Shutdown();
            return false;
        }

        positionAttribute_ = glGetAttribLocation(program_, "aPosition");
        resolutionUniform_ = glGetUniformLocation(program_, "uResolution");
        colorUniform_ = glGetUniformLocation(program_, "uColor");
        if (positionAttribute_ < 0 || resolutionUniform_ < 0 || colorUniform_ < 0) {
            std::cerr << "Could not locate UI shader inputs.\n";
            Shutdown();
            return false;
        }

        glGenBuffers(1, &vertexBuffer_);
        if (vertexBuffer_ == 0) {
            std::cerr << "Could not create UI vertex buffer.\n";
            Shutdown();
            return false;
        }
        return true;
    }

    void BeginFrame(int width, int height, int drawableWidth, int drawableHeight)
    {
        glViewport(0, 0, drawableWidth, drawableHeight);
        glClearColor(0.055f, 0.065f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program_);
        glUniform2f(resolutionUniform_,
                    static_cast<GLfloat>(width),
                    static_cast<GLfloat>(height));
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glEnableVertexAttribArray(static_cast<GLuint>(positionAttribute_));
    }

    void DrawRectangle(GLfloat x,
                       GLfloat y,
                       GLfloat width,
                       GLfloat height,
                       Color color)
    {
        std::vector<GLfloat> vertices;
        AppendRectangle(vertices, x, y, width, height);
        DrawVertices(vertices, color);
    }

    GLfloat DrawText(const std::string& text,
                     GLfloat x,
                     GLfloat y,
                     GLfloat scale,
                     GLfloat maxX,
                     Color color)
    {
        std::vector<GLfloat> vertices;
        GLfloat characterX = x;
        for (const char character : text) {
            if (characterX + 5.0f * scale > maxX) {
                break;
            }
            const auto rows = GlyphRows(character);
            for (std::size_t row = 0; row < rows.size(); ++row) {
                for (std::size_t column = 0; column < 5; ++column) {
                    if ((rows[row] & (1u << (4u - column))) != 0) {
                        AppendRectangle(vertices,
                                        characterX + static_cast<GLfloat>(column) * scale,
                                        y + static_cast<GLfloat>(row) * scale,
                                        scale,
                                        scale);
                    }
                }
            }
            characterX += 6.0f * scale;
        }
        DrawVertices(vertices, color);
        return characterX;
    }

    void Shutdown()
    {
        if (vertexBuffer_ != 0) {
            glDeleteBuffers(1, &vertexBuffer_);
            vertexBuffer_ = 0;
        }
        if (program_ != 0) {
            glDeleteProgram(program_);
            program_ = 0;
        }
    }

private:
    GLuint CompileShader(GLenum type, const char* source)
    {
        const GLuint shader = glCreateShader(type);
        if (shader == 0) {
            std::cerr << "Could not create UI shader.\n";
            return 0;
        }
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled != GL_TRUE) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> log(static_cast<std::size_t>(std::max(logLength, 1)));
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            std::cerr << "Could not compile UI shader: " << log.data() << '\n';
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    void PrintProgramLog(GLuint program)
    {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(static_cast<std::size_t>(std::max(logLength, 1)));
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        std::cerr << "Could not link UI shader: " << log.data() << '\n';
    }

    void DrawVertices(const std::vector<GLfloat>& vertices, Color color)
    {
        if (vertices.empty()) {
            return;
        }
        glUniform4f(colorUniform_, color.r, color.g, color.b, color.a);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size() * sizeof(GLfloat)),
                     vertices.data(),
                     GL_STREAM_DRAW);
        glVertexAttribPointer(static_cast<GLuint>(positionAttribute_),
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              nullptr);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 2));
    }

    GLuint program_ = 0;
    GLuint vertexBuffer_ = 0;
    GLint positionAttribute_ = -1;
    GLint resolutionUniform_ = -1;
    GLint colorUniform_ = -1;
};

struct Button {
    int x;
    int y;
    int width;
    int height;
    const char* label;
    Color color;
};

bool Contains(const Button& button, int x, int y)
{
    return x >= button.x && x < button.x + button.width &&
           y >= button.y && y < button.y + button.height;
}

void RenderUi(UiRenderer& renderer,
              int width,
              int height,
              int drawableWidth,
              int drawableHeight,
              const std::string& url,
              const std::string& status,
              bool urlFocused)
{
    constexpr int margin = 20;
    constexpr int urlY = 20;
    constexpr int urlHeight = 44;
    constexpr int buttonY = 80;
    constexpr int buttonWidth = 92;
    constexpr int buttonHeight = 38;
    constexpr int buttonGap = 10;
    constexpr int videoY = 164;

    const Color white{0.90f, 0.92f, 0.96f, 1.0f};
    const Color muted{0.55f, 0.60f, 0.68f, 1.0f};
    const Color field{0.12f, 0.14f, 0.18f, 1.0f};
    const Color border{0.30f, 0.36f, 0.44f, 1.0f};
    const Color playColor{0.12f, 0.42f, 0.28f, 1.0f};
    const Color pauseColor{0.48f, 0.34f, 0.10f, 1.0f};
    const Color stopColor{0.48f, 0.16f, 0.16f, 1.0f};
    const Color videoColor{0.015f, 0.02f, 0.03f, 1.0f};

    renderer.BeginFrame(width, height, drawableWidth, drawableHeight);
    renderer.DrawRectangle(margin, urlY, width - 2 * margin, urlHeight, border);
    renderer.DrawRectangle(margin + 2, urlY + 2, width - 2 * margin - 4,
                           urlHeight - 4, field);

    const std::string visibleUrl = url.empty() ? "ENTER STREAM URL" : url;
    const Color urlColor = url.empty() ? muted : white;
    const GLfloat textX = static_cast<GLfloat>(margin + 12);
    const GLfloat textY = static_cast<GLfloat>(urlY + 15);
    const GLfloat textEnd = url.empty()
        ? textX
        : renderer.DrawText(visibleUrl, textX, textY, 2.0f,
                            static_cast<GLfloat>(width - margin - 10), urlColor);
    if (url.empty()) {
        renderer.DrawText(visibleUrl, textX, textY, 2.0f,
                          static_cast<GLfloat>(width - margin - 10), urlColor);
    }
    if (urlFocused && (SDL_GetTicks() / 500u) % 2u == 0u) {
        renderer.DrawRectangle(textEnd + 2.0f, textY, 2.0f, 14.0f, white);
    }

    const Button buttons[] = {
        {margin, buttonY, buttonWidth, buttonHeight, "PLAY", playColor},
        {margin + buttonWidth + buttonGap, buttonY, buttonWidth,
         buttonHeight, "PAUSE", pauseColor},
        {margin + 2 * (buttonWidth + buttonGap), buttonY, buttonWidth,
         buttonHeight, "STOP", stopColor}
    };
    for (const Button& button : buttons) {
        renderer.DrawRectangle(button.x, button.y, button.width, button.height,
                               button.color);
        const GLfloat labelWidth = static_cast<GLfloat>(std::string(button.label).size() * 12);
        renderer.DrawText(button.label,
                          button.x + (button.width - labelWidth) / 2.0f,
                          button.y + 12.0f,
                          2.0f,
                          static_cast<GLfloat>(button.x + button.width),
                          white);
    }

    renderer.DrawText("STATUS:", margin, 136.0f, 2.0f,
                      static_cast<GLfloat>(width - margin), muted);
    renderer.DrawText(status, 112.0f, 136.0f, 2.0f,
                      static_cast<GLfloat>(width - margin), white);

    const int videoHeight = std::max(80, height - videoY - margin);
    renderer.DrawRectangle(margin, videoY, width - 2 * margin, videoHeight, border);
    renderer.DrawRectangle(margin + 2, videoY + 2, width - 2 * margin - 4,
                           videoHeight - 4, videoColor);
    renderer.DrawText("VIDEO OUTPUT AREA",
                      static_cast<GLfloat>(width / 2 - 102),
                      static_cast<GLfloat>(videoY + videoHeight / 2 - 7),
                      2.0f,
                      static_cast<GLfloat>(width - margin),
                      muted);
}

} // namespace

int main()
{
    SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "1");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << '\n';
        return 1;
    }

    const SDL_GLattr attributes[] = {
        SDL_GL_CONTEXT_MAJOR_VERSION,
        SDL_GL_CONTEXT_MINOR_VERSION,
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_DOUBLEBUFFER
    };
    const int values[] = {2, 0, SDL_GL_CONTEXT_PROFILE_ES, 1};
    for (std::size_t index = 0; index < std::size(attributes); ++index) {
        if (SDL_GL_SetAttribute(attributes[index], values[index]) != 0) {
            std::cerr << "Could not configure the OpenGL ES context: "
                      << SDL_GetError() << '\n';
            SDL_Quit();
            return 1;
        }
    }

    SDL_Window* window = SDL_CreateWindow(
        "pi-player-simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        960,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "Could not create simulator window: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowMinimumSize(window, 480, 320);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "Could not create OpenGL ES context: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    UiRenderer renderer;
    if (!renderer.Initialize()) {
        renderer.Shutdown();
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    if (SDL_GL_SetSwapInterval(1) != 0) {
        std::cerr << "Could not enable VSync: " << SDL_GetError() << '\n';
    }

    std::string url;
    std::string status = "Ready. Playback is mocked.";
    bool urlFocused = false;
    bool running = true;
    const Button buttons[] = {
        {20, 80, 92, 38, "PLAY", {0.12f, 0.42f, 0.28f, 1.0f}},
        {122, 80, 92, 38, "PAUSE", {0.48f, 0.34f, 0.10f, 1.0f}},
        {224, 80, 92, 38, "STOP", {0.48f, 0.16f, 0.16f, 1.0f}}
    };

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
                urlFocused = mouseX >= 20 && mouseX < windowWidth - 20 &&
                             mouseY >= 20 && mouseY < std::min(64, windowHeight);
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
                if (url.size() + std::string(event.text.text).size() <= 512) {
                    url += event.text.text;
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
        int drawableWidth = 0;
        int drawableHeight = 0;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
        if (drawableWidth > 0 && drawableHeight > 0) {
            RenderUi(renderer, width, height, drawableWidth, drawableHeight,
                     url, status, urlFocused);
            SDL_GL_SwapWindow(window);
        }
        SDL_Delay(16);
    }

    SDL_StopTextInput();
    renderer.Shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
