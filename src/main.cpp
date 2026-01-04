#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>
#include <string>

// ReSharper disable CppUseStructuredBinding
// ReSharper disable CppParameterMayBeConstPtrOrRef
// ReSharper disable CppUseStructuredBinding
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppVariableCanBeMadeConstexpr

namespace {

struct SDLWindowDeleter
{
    void operator()(SDL_Window* window) const
    {
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        }
    }
};

struct SDLRendererDeleter
{
    void operator()(SDL_Renderer* renderer) const
    {
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
        }
    }
};

class AppState
{
public:
    AppState(int width, int height, int scale = 4)
        : cellCols(width), cellRows(height), cellScale(scale)
    {
    }

    SDL_Window* setWindow(SDL_Window* window)
    {
        m_window.reset(window);
        return m_window.get();
    }

    [[nodiscard]]
    SDL_Window* getWindow() const
    {
        return m_window.get();
    }

    SDL_Renderer* setRenderer(SDL_Renderer* renderer)
    {
        m_renderer.reset(renderer);
        return m_renderer.get();
    }

    [[nodiscard]]
    SDL_Renderer* getRenderer() const
    {
        return m_renderer.get();
    }

    const int cellCols;
    const int cellRows;
    const int cellScale;

    const int cellSize {7};

    uint64_t timeStartFps {};
    int fps {60};

private:
    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> m_renderer;
};
} // namespace

extern "C" {
SDL_AppResult SDL_AppInit(void** appstate, int, char**)
{
    const int cellCols {24};
    const int cellRows {16};
    const int cellScale {5};

    *appstate = new AppState(cellCols, cellRows, cellScale);
    auto& state {*static_cast<AppState*>(*appstate)};

    const std::string title {"Cppel7"};
    const std::string appName {title + ": A fantasy console"};
    const std::string appVersion {"0.1.0"};
    const std::string appIdentifier {"com.liconaj.cppel7"};
    const int MIN_WINDOW_WIDTH {state.cellCols * state.cellSize};
    const int MIN_WINDOW_HEIGHT {state.cellRows * state.cellSize};

    const int WINDOW_WIDTH {MIN_WINDOW_WIDTH * state.cellScale};
    const int WINDOW_HEIGHT {MIN_WINDOW_HEIGHT * state.cellScale};

    SDL_SetAppMetadata(appName.c_str(), appVersion.c_str(), appIdentifier.c_str());

    SDL_Log("SDL_Init: initializing video and events subsystems");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == false) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Log("SDL_CreateWindow: Initializing SDL_Window");
    SDL_Window* window {state.setWindow(SDL_CreateWindow(title.c_str(), WINDOW_WIDTH, WINDOW_HEIGHT,
                                                         SDL_WINDOW_RESIZABLE))};
    if (window == nullptr) {
        SDL_Log("Couldn't create SDL_Window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

    SDL_Log("SDL_CreateRenderer: Initializing SDL_Renderer");
    SDL_Renderer* renderer {state.setRenderer(SDL_CreateRenderer(window, nullptr))};
    if (renderer == nullptr) {
        SDL_Log("Couldn't create SDL_Renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Make the renderer content scaled up by integer multiples to fit the output resolution
    SDL_SetRenderLogicalPresentation(renderer, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    state.timeStartFps = SDL_GetTicksNS();
    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(state.fps).c_str());

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto& state {*static_cast<AppState*>(appstate)};

    SDL_Renderer* renderer {state.getRenderer()};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // --------------------------------------
    // Begin drawing

    int w, h;
    SDL_GetRenderLogicalPresentation(renderer, &w, &h, nullptr);
    const SDL_FRect rect {0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h)};

    SDL_SetRenderDrawColor(renderer, 100, 149, 237, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);

    // End drawing
    // --------------------------------------
    SDL_RenderPresent(renderer);

    // Adjust FPS
    const double framesPerNs {1e9 / state.fps};
    if (const uint64_t timePassedNs {SDL_GetTicksNS() - state.timeStartFps};
        timePassedNs < framesPerNs) {
        SDL_DelayNS(framesPerNs - timePassedNs);
    }
    state.timeStartFps = SDL_GetTicksNS();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event)
{
    if (event == nullptr) {
        SDL_Log("SDL_Event: Couldn't get event");
        return SDL_APP_FAILURE;
    }

    if (event->type == SDL_EVENT_QUIT) {
        SDL_Log("SDL_Event: SDL_EVENT_QUIT");
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appState, SDL_AppResult)
{
    delete static_cast<AppState*>(appState);
}

} // extern "C"