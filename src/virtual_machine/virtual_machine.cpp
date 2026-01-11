#include "virtual_machine.h"

namespace cppel7 {

VirtualMachine::VirtualMachine(const size_t size) : m_memory(size)
{
}

std::byte VirtualMachine::peek(const std::size_t address) const
{
    return m_memory.at(address);
}

std::span<const std::byte> VirtualMachine::peek(const std::size_t address, const size_t n) const
{
    return {m_memory.data() + address, n};
}

void VirtualMachine::poke(const std::size_t address, const std::byte value)
{
    m_memory.at(address) = value;
}

void VirtualMachine::poke(const std::size_t address, std::span<const std::byte> values)
{
    std::copy_n(values.begin(), values.size(), m_memory.begin() + address);
}

std::span<const std::byte> VirtualMachine::memory() const
{
    return m_memory;
}

const std::byte* VirtualMachine::data() const
{
    return m_memory.data();
}
} // namespace cppel7