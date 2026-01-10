#include "engine.h"

namespace cppel7 {

Engine::Engine()
{
    m_frameBuffer = std::make_unique<FrameBuffer>(m_config.width, m_config.height);
}

const Config& Engine::config()
{
    return m_config;
}

FrameBuffer& Engine::frameBuffer()
{
    return *m_frameBuffer;
}

const FrameBuffer& Engine::frameBuffer() const
{
    return *m_frameBuffer;
}

void Engine::requestQuit()
{
    m_quitRequested = true;
}

bool Engine::quitRequested() const
{
    return m_quitRequested;
}

} // namespace  cppel7