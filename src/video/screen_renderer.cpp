#include "screen_renderer.h"

#include "core/constants.h"
#include "video_memory_view.h"

#include <cstring>

namespace cppel7 {

ScreenRenderer::ScreenRenderer(const VideoMemoryView& videoMemoryView)
    : m_videoMemoryView(videoMemoryView)
{
}

void ScreenRenderer::render(FrameBuffer& frameBuffer) const
{
    const std::span cells {m_videoMemoryView.cells()};
    for (Size cellIndex {}; cellIndex < cells.size(); ++cellIndex) {
        const Size cellY {cellIndex / m_videoMemoryView.screenWidth()};
        const Size cellX {cellIndex % m_videoMemoryView.screenWidth()};
        drawCell(frameBuffer, cells[cellIndex], cellX, cellY);
    }
}

void ScreenRenderer::drawCell(FrameBuffer& frameBuffer, const Cell& cell, const Size cellX,
                              const Size cellY) const
{
    const auto& [glyphIndex, colorAttr] {cell};

    const PixelColor fgColor {m_videoMemoryView.paletteColor(colorAttr.foreground())};
    const PixelColor bgColor {m_videoMemoryView.paletteColor(colorAttr.background())};

    for (Size glyphPixelIndex {}; glyphPixelIndex < FONT_ATLAS_BYTES_PER_GLYPH; ++glyphPixelIndex) {

        const Size glyphPixelX {glyphPixelIndex % FONT_GLYPH_WIDTH};
        const Size glyphPixelY {glyphPixelIndex / FONT_GLYPH_WIDTH};

        const Size fx {(cellX * FONT_GLYPH_WIDTH) + glyphPixelX};
        const Size fy {(cellY * FONT_GLYPH_HEIGHT) + glyphPixelY};

        const Byte pixelValue {m_videoMemoryView.glyphPixelValue(glyphIndex, glyphPixelIndex)};
        const PixelColor pixel {pixelValue == 0x1 ? fgColor : bgColor};
        frameBuffer.setPixel(fx, fy, pixel);
    }
}

} // namespace cppel7