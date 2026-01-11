#include "screen_renderer.h"
#include "core/constants.h"

#include <cstring>

namespace cppel7 {

ScreenRenderer::ScreenRenderer(const Screen& screen)
    : m_screen(screen), m_virtualMachine(screen.virtualMachine())
{
}

void ScreenRenderer::render(FrameBuffer& frameBuffer) const
{
    const std::span cells {m_screen.cells()};
    for (Size cellIndex {}; cellIndex < m_screen.size(); ++cellIndex) {
        const Size cellY {cellIndex / m_screen.width()};
        const Size cellX {cellIndex % m_screen.width()};
        drawCell(frameBuffer, cells[cellIndex], cellX, cellY);
    }
}

void ScreenRenderer::drawCell(FrameBuffer& frameBuffer, const Cell& cell, const Size cellX,
                              const Size cellY) const
{
    const auto& [glyphIndex, colorAttr] {cell};

    const PixelColor fgColor {getPaletteColor(colorAttr.foreground())};
    const PixelColor bgColor {getPaletteColor(colorAttr.background())};

    for (Size glyphPixelIndex {}; glyphPixelIndex < FONT_ATLAS_BYTES_PER_GLYPH; ++glyphPixelIndex) {

        const Size glyphPixelX {glyphPixelIndex % FONT_GLYPH_WIDTH};
        const Size glyphPixelY {glyphPixelIndex / FONT_GLYPH_WIDTH};

        const Size fx {(cellX * FONT_GLYPH_WIDTH) + glyphPixelX};
        const Size fy {(cellY * FONT_GLYPH_HEIGHT) + glyphPixelY};

        const PixelColor pixel {isGlyphPixelSet(glyphIndex, glyphPixelIndex) ? fgColor : bgColor};
        frameBuffer.setPixel(fx, fy, pixel);
    }
}

PixelColor ScreenRenderer::getPaletteColor(const PaletteIndex index) const
{
    const Address paletteColorAddress {ADDR_PALETTE_BASE + (index * PALETTE_BYTES_PER_COLOR)};

    PixelColor color {};
    std::memcpy(&color, m_virtualMachine.data() + paletteColorAddress, PALETTE_BYTES_PER_COLOR);

    return color;
}

bool ScreenRenderer::isGlyphPixelSet(const GlyphIndex glyph, const Size pixelIndex) const
{
    const Address address {ADDR_FONT_ATLAS_BASE
                           + static_cast<Address>(glyph) * FONT_ATLAS_BYTES_PER_GLYPH};
    return (m_virtualMachine.peek(address + pixelIndex) & 0x01) != 0;
}

} // namespace cppel7