#ifndef CPPEL7_SCREEN_H
#define CPPEL7_SCREEN_H

#include <optional>

#include "core/config.h"
#include "core/types.h"
#include "virtual_machine/virtual_machine.h"

namespace cppel7 {

class DrawContext
{
public:
    DrawContext(VirtualMachine& virtualMachine, const Config& config);

    void setColor(ColorAttr color);

    void put(int x, int y, GlyphIndex glyph) const;
    void put(int x, int y, std::span<GlyphIndex> glyphs) const;

    void fill(int x, int y, int w, int h, GlyphIndex glyph) const;

    [[nodiscard]]
    std::optional<Cell> get(int x, int y) const;

private:
    VirtualMachine& m_virtualMachine;
    const Size m_width;
    const Size m_height;

    ColorAttr m_currentColor {};

    [[nodiscard]]
    bool checkCellPosition(int x, int y) const;

    [[nodiscard]]
    std::optional<Address> getCellAddress(int x, int y) const;
};

} // namespace cppel7

#endif // CPPEL7_SCREEN_H
