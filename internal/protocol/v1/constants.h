#pragma once

#include <cstddef>
#include <cstdint>

namespace dts::protocol::v1 {

    inline constexpr std::uint32_t kMagic = 0x44545331;
    inline constexpr std::uint16_t kVersion = 1;

    inline constexpr std::size_t kSymbolLen = 16;
    inline constexpr std::size_t kClientOrderIdLen = 36;

    inline constexpr std::size_t kInstructionWireBytes = 120;

}