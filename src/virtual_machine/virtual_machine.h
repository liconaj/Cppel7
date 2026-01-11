#ifndef CPPEL7_VIRTUAL_MACHINE_H
#define CPPEL7_VIRTUAL_MACHINE_H

#include <span>
#include <vector>

#include "core/types.h"

namespace cppel7 {

class VirtualMachine
{
public:
    explicit VirtualMachine(Size size);

    [[nodiscard]]
    Byte peek(Address address) const;

    [[nodiscard]]
    std::span<const Byte> peek(Address address, Size n) const;

    void poke(Address address, Byte value);

    void poke(Address address, std::span<const Byte> values);

    [[nodiscard]]
    std::span<const Byte> memory() const;

    [[nodiscard]]
    const Byte* data() const;

private:
    std::vector<Byte> m_memory;
};

} // namespace cppel7

#endif // CPPEL7_VIRTUAL_MACHINE_H
