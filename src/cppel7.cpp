#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "platform/app_state.h"

// ReSharper disable CppUseStructuredBinding
// ReSharper disable CppParameterMayBeConstPtrOrRef
// ReSharper disable CppUseStructuredBinding
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppVariableCanBeMadeConstexpr
// ReSharper disable CppRedundantParentheses


extern "C" {

SDL_AppResult SDL_AppInit(void** appstate, int, char**)
{
    const auto appName {"Cppel7: A framework for making grid based games"};
    const auto appVersion {"0.1.0"};
    const auto appIdentifier {"com.liconaj.cppel7"};

    SDL_SetAppMetadata(appName, appVersion, appIdentifier);

    *appstate = new cppel7::AppState();
    auto& state {*static_cast<cppel7::AppState*>(*appstate)};

    try {
        state.video().initialize(state.engine().config());
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        return SDL_APP_FAILURE;
    }

    state.startFpsTimer();

    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto& state {*static_cast<cppel7::AppState*>(appstate)};

    state.engine().step();
    state.engine().render();
    state.video().present(state.engine().frameBuffer());
    state.adjustFps();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    auto& state {*static_cast<cppel7::AppState*>(appstate)};

    state.input().handleEvent(*event);

    if (state.engine().quitRequested() == true) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult)
{
    delete static_cast<cppel7::AppState*>(appstate);
}

} // extern "C"