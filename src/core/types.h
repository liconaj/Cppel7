#ifndef CPPEL7_CELL_H
#define CPPEL7_CELL_H

#include <cstddef>

namespace cppel7 {

using Address = int;

using GlyphIndex = std::byte;

using PaletteIndex = int;

struct ColorAttr
{
    std::byte value;

    [[nodiscard]]
    PaletteIndex foreground() const noexcept
    {
        return static_cast<PaletteIndex>(value & std::byte{0x0F});
    }

    [[nodiscard]]
    PaletteIndex background() const noexcept
    {
        return static_cast<PaletteIndex>((value & std::byte{0xF0}) >> 4);
    }
};

struct Cell
{
    GlyphIndex glyph;
    ColorAttr colorAttr;
};

} // namespace cppel7

#endif // CPPEL7_CELL_H
