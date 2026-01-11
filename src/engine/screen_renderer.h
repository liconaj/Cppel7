#ifndef CPPEL7_SCREEN_RENDERER_H
#define CPPEL7_SCREEN_RENDERER_H

#include "framebuffer.h"
#include "screen.h"

namespace cppel7 {

class ScreenRenderer
{
public:
    explicit ScreenRenderer(const Screen& screen);

    void render(FrameBuffer& frameBuffer) const;

private:
    const Screen& m_screen;
    const VirtualMachine& m_virtualMachine;

    void drawCell(FrameBuffer& frameBuffer, const Cell& cell, Size cellX, Size cellY) const;

    [[nodiscard]]
    PixelColor getPaletteColor(PaletteIndex index) const;

    [[nodiscard]]
    bool isGlyphPixelSet(GlyphIndex glyph, Size pixelIndex) const;
};

} // namespace cppel7

#endif // CPPEL7_SCREEN_RENDERER_H
