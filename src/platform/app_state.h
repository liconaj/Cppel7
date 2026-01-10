#ifndef APP_STATE_H
#define APP_STATE_H

#include "SDL3/SDL.h"
#include "sdl_input.h"
#include "sdl_video.h"

namespace cppel7 {

class AppState
{
public:
    AppState();

    Engine& engine();
    SdlVideo& video();
    SdlInput& input();

    void adjustFps();
    void startFpsTimer();

private:
    Engine m_engine {};

    SdlVideo m_video {};
    SdlInput m_input;

    std::uint64_t m_timeStartFpsNs {};
};


} // namespace cppel7

#endif // APP_STATE_H