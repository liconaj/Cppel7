#ifndef CPPEL7_SCREEN_H
#define CPPEL7_SCREEN_H

#include "core/types.h"
#include "virtual_machine/virtual_machine.h"

#include <optional>

namespace cppel7 {

class Screen
{
public:
    Screen(VirtualMachine& virtualMachine, Size width, Size height);

    void setColor(ColorAttr color);

    void put(int x, int y, GlyphIndex glyph) const;
    void put(int x, int y, std::span<GlyphIndex> glyphs) const;

    void fill(int x, int y, int w, int h, GlyphIndex glyph) const;

    [[nodiscard]]
    std::optional<Cell> get(int x, int y) const;

    [[nodiscard]]
    Size width() const;

    [[nodiscard]]
    Size height() const;

    [[nodiscard]]
    Size size() const;

    [[nodiscard]]
    std::span<const Cell> cells() const;

    [[nodiscard]]
    const VirtualMachine& virtualMachine() const;

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
