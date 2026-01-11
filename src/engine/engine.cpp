#include "engine.h"

#include "core/color_palette.h"
#include "core/constants.h"
#include "core/font_atlas.h"
#include "core/types.h"

namespace cppel7 {

Engine::Engine()
{
    m_config.title = "Hello Cppel7";
    m_config.width = 12;
    m_config.height = 8;
    m_config.scale = 4;
    m_config.debug = true;

    m_frameBuffer = std::make_unique<FrameBuffer>(m_config.width * CELL_SIZE,
                                                  m_config.height * CELL_SIZE);

    const Size cells {m_config.width * m_config.height};
    const Size screenBufferSize {cells * SCREEN_BUFFER_BYTES_PER_CELL};
    const Size memorySize {ADDR_SCREEN_BUFFER_BASE + screenBufferSize};
    m_virtualMachine = std::make_unique<VirtualMachine>(memorySize);

    m_screen = std::make_unique<Screen>(*m_virtualMachine, m_config.width, m_config.height);
    m_screenRenderer = std::make_unique<ScreenRenderer>(*m_screen);

    uploadDefaultPalette();
    uploadDefaultFontAtlas();
}

void Engine::step() const
{
    for (Size i {}; i < FONT_ATLAS_GLYPH_COUNT; ++i) {
        const Size x {i % m_config.width};
        const Size y {i / m_config.width};

        constexpr int COLOR_OFFSET {2};
        const auto bg {static_cast<Byte>((i + COLOR_OFFSET) % PALETTE_COLOR_COUNT)};
        const auto fg {static_cast<Byte>((i + COLOR_OFFSET + 2) % PALETTE_COLOR_COUNT)};

        m_screen->setColor(ColorAttr {static_cast<Byte>(fg | (bg << 4))});
        m_screen->put(x, y, static_cast<PaletteIndex>(i));
    }
}

void Engine::render() const
{
    m_screenRenderer->render(*m_frameBuffer);
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