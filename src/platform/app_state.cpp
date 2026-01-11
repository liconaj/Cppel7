#include "app_state.h"
#include "core/constants.h"

#include <sstream>
#include <string>

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

    const std::uint64_t nowNs {SDL_GetTicksNS()};

    m_statsTimerNs = nowNs;
    m_timeAccumulator = 0.0;

    initDebugStats(nowNs);

    return SDL_APP_CONTINUE;
}

SDL_AppResult AppState::onIterate()
{
    const std::uint64_t currentTicksNs {SDL_GetTicksNS()};
    const double deltaSeconds {(currentTicksNs - m_lastTickNs) * 1e-9};

    m_lastTickNs = currentTicksNs;

    tick(deltaSeconds);
    renderFrame();

    if (m_engine.config().debug) {
        updateDebugStats(currentTicksNs);
    }

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

void AppState::tick(double deltaSeconds)
{
    m_timeAccumulator += deltaSeconds;

    constexpr double stepDt {1.0 / TPS};
    constexpr int maxStepsPerIter {5};

    int stepsExecuted {0};
    while (m_timeAccumulator >= stepDt && stepsExecuted < maxStepsPerIter) {
        m_engine.step();
        ++m_stepCount;

        m_timeAccumulator -= stepDt;
        ++stepsExecuted;
    }
}

void AppState::renderFrame()
{
    m_engine.render();
    m_video.present(m_engine.frameBuffer());
    ++m_frameCount;
}

void AppState::initDebugStats(const std::uint64_t currentTicksNs)
{
    m_lastTickNs = currentTicksNs;
    m_stepCount = 0;
    m_frameCount = 0;

    showDebugStats(0.0, 0.0);
}

void AppState::updateDebugStats(const std::uint64_t currentTicksNs)
{
    const std::uint64_t elapsedNs {currentTicksNs - m_statsTimerNs};
    if (constexpr std::size_t UPDATE_DEBUG_TIME_NS {1'000'000'000ULL};
        elapsedNs < UPDATE_DEBUG_TIME_NS) {
        return;
    }

    const double seconds {elapsedNs * 1e-9};
    const double tps {m_stepCount / seconds};
    const double fps {m_frameCount / seconds};
    showDebugStats(tps, fps);

    m_statsTimerNs = currentTicksNs;
    m_stepCount = 0;
    m_frameCount = 0;
}

void AppState::showDebugStats(const double tps, const double fps)
{
    std::ostringstream title;
    title.setf(std::ios::fixed);
    title.precision(1);
    title << m_engine.config().title << " [DEBUG] - " << tps << " TPS | " << fps << " FPS";
    m_video.updateWindowTitle(title.str());
}

} // namespace cppel7