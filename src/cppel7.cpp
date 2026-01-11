#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "platform/app_state.h"

extern "C" {

SDL_AppResult SDL_AppInit(void** appstate, int, char**)
{
    const auto appName {"Cppel7: A framework for making grid based games"};
    const auto appVersion {"0.1.0"};
    const auto appIdentifier {"com.liconaj.cppel7"};

    SDL_SetAppMetadata(appName, appVersion, appIdentifier);

    *appstate = new cppel7::AppState();
    auto& state {*static_cast<cppel7::AppState*>(*appstate)};

    return state.onInit();
}
SDL_AppResult SDL_AppIterate(void* appstate)
{
    auto& state {*static_cast<cppel7::AppState*>(appstate)};
    return state.onIterate();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    const auto& state {*static_cast<cppel7::AppState*>(appstate)};
    return state.onEvent(*event);
}

void SDL_AppQuit(void* appstate, SDL_AppResult)
{
    delete static_cast<cppel7::AppState*>(appstate);
}

} // extern "C"