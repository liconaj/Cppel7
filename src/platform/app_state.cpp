#include "app_state.h"
#include "core/constants.h"

namespace cppel7 {
AppState::AppState() : m_input(m_engine)
{
}

SDL_AppResult AppState::onInit()
{
    try {
        m_video.initialize(m_engine.config());
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
        return SDL_APP_FAILURE;
    }

    m_lastTickNs = SDL_GetTicksNS();
    m_timeAccumulator = 0;

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppState::onIterate()
{
    const std::uint64_t nowNs {SDL_GetTicksNS()};

    const std::uint64_t currentTickNs {nowNs};
    const double deltaSeconds {(currentTickNs - m_lastTickNs) * 1e-9};

    m_lastTickNs = currentTickNs;
    m_timeAccumulator += deltaSeconds;

    constexpr double stepDt {1.0 / TPS};
    constexpr int maxStepsPerIter {5};

    int stepsExecuted {0};
    while (m_timeAccumulator >= stepDt && stepsExecuted < maxStepsPerIter) {
        m_engine.step();
        m_timeAccumulator -= stepDt;
        ++stepsExecuted;
    }

    m_engine.render();
    m_video.present(m_engine.frameBuffer());

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppState::onEvent(SDL_Event const& event) const
{
    m_input.handleEvent(event);
    if (m_engine.quitRequested() == true) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

} // namespace cppel7