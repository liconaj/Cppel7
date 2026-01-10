#include "app_state.h"

#include "core/constants.h"

namespace cppel7 {

AppState::AppState()
    : m_input(m_engine)
{
    m_timeStartFpsNs = SDL_GetTicksNS();
}

Engine& AppState::engine()
{
    return m_engine;
}

SdlVideo& AppState::video()
{
    return m_video;
}

SdlInput& AppState::input()
{
    return m_input;
}

void AppState::adjustFps()
{
    constexpr double framePerNs {1e9 / FPS};
    if (const uint64_t timePassedNs {SDL_GetTicksNS() - m_timeStartFpsNs};
        timePassedNs < framePerNs) {
        SDL_DelayNS(framePerNs - timePassedNs);
    }
    m_timeStartFpsNs = SDL_GetTicksNS();
}

} // namespace cppel7