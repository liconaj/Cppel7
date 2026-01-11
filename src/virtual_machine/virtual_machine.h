#ifndef CPPEL7_VIRTUAL_MACHINE_H
#define CPPEL7_VIRTUAL_MACHINE_H

#include <cstddef>
#include <vector>
#include <span>

namespace cppel7 {

class VirtualMachine
{
public:
    explicit VirtualMachine(size_t size);

    [[nodiscard]]
    std::byte peek(std::size_t address) const;

    [[nodiscard]]
    std::span<const std::byte> peek(std::size_t address, size_t n) const;

    void poke(std::size_t address, std::byte value);

    void poke(std::size_t address, std::span<const std::byte> values);

    [[nodiscard]]
    std::span<const std::byte> memory() const;

    [[nodiscard]]
    const std::byte* data() const;

private:
    std::vector<std::byte> m_memory;
};

} // namespace cppel7

#endif // CPPEL7_VIRTUAL_MACHINE_H
