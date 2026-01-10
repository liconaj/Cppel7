#include "engine.h"

#include "core/color_palette.h"
#include "core/constants.h"
#include "core/font_atlas.h"

namespace cppel7 {

Engine::Engine()
{
    m_frameBuffer = std::make_unique<FrameBuffer>(m_config.width, m_config.height);

    const int cells {m_config.width * m_config.height};
    const int screenBufferSize {cells * SCREEN_BUFFER_BYTES_PER_CELL};
    const int memorySize {ADDR_FIXED_MEMORY_END + screenBufferSize};
    m_virtualMachine = std::make_unique<VirtualMachine>(memorySize);

    uploadDefaultPalette();
    uploadDefaultFontAtlas();
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

void Engine::uploadDefaultFontAtlas() const
{
    if (m_virtualMachine == nullptr) {
        return;
    }

    m_virtualMachine->poke(ADDR_FONT_ATLAS_BASE, DEFAULT_FONT_ATLAS);
}

void Engine::uploadDefaultPalette() const
{
    if (m_virtualMachine == nullptr) {
        return;
    }

    m_virtualMachine->poke(ADDR_PALETTE_BASE, DEFAULT_PALETTE);
}

} // namespace  cppel7