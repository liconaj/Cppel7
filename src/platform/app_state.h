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

    void tick(double deltaSeconds);
    void renderFrame();

    std::uint64_t m_statsTimerNs {};
    int m_stepCount {};
    int m_frameCount {};

    void initDebugStats(std::uint64_t currentTicksNs);
    void updateDebugStats(std::uint64_t currentTicksNs);
    void showDebugStats(double tps, double fps);
};

} // namespace cppel7

#endif // APP_STATE_H