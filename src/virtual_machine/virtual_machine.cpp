#include <algorithm>

#include "virtual_machine.h"

namespace cppel7 {

VirtualMachine::VirtualMachine(const Size size) : m_memory(size)
{
}

Byte VirtualMachine::peek(const Address address) const
{
    return m_memory.at(address);
}

std::span<const Byte> VirtualMachine::peek(const Address address, const Size n) const
{
    return {m_memory.data() + address, n};
}

void VirtualMachine::poke(const Address address, const Byte value)
{
    m_memory.at(address) = value;
}

void VirtualMachine::poke(const Address address, std::span<const Byte> values)
{
    std::copy_n(values.begin(), values.size(), m_memory.begin() + address);
}

std::span<const Byte> VirtualMachine::memory() const
{
    return m_memory;
}

const Byte* VirtualMachine::data() const
{
    return m_memory.data();
}
} // namespace cppel7