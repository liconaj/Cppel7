#ifndef APP_STATE_H
#define APP_STATE_H

#include "sdl_input.h"
#include "sdl_video.h"

namespace cppel7 {

class AppState
{
public:
    AppState();

    SDL_AppResult onInit();
    SDL_AppResult onIterate();
    SDL_AppResult onEvent(SDL_Event const& event) const;

private:
    Engine m_engine;

    SdlVideo m_video;
    SdlInput m_input;

    std::uint64_t m_lastTickNs {};
    double m_timeAccumulator {};
};

} // namespace cppel7

#endif // APP_STATE_H