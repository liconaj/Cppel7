#ifndef CPPEL7_VIDEO_MEMORY_VIEW_H
#define CPPEL7_VIDEO_MEMORY_VIEW_H

#include "framebuffer.h"
#include "virtual_machine/virtual_machine.h"

namespace cppel7 {

class VideoMemoryView
{
public:
    explicit VideoMemoryView(const VirtualMachine& virtualMachine, Size screenWidth,
                             Size screenHeight);

    [[nodiscard]]
    std::span<const Cell> cells() const;

    [[nodiscard]]
    PixelColor paletteColor(PaletteIndex index) const;

    [[nodiscard]]
    Byte glyphPixelValue(GlyphIndex glyphIndex, Size pixelIndex) const;

    [[nodiscard]]
    Size screenWidth() const;

    [[nodiscard]]
    Size screenHeight() const;

private:
    const VirtualMachine& m_virtualMachine;
    const Size m_screenWidth;
    const Size m_screenHeight;
};

} // namespace cppel7

#endif // CPPEL7_VIDEO_MEMORY_VIEW_H
