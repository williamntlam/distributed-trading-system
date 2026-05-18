#pragma once

#include <cstdint>

namespace dts::protocol::v1 {

    enum class Opcode : std::uint8_t {

        kInvalid = 0,
        kNewOrder = 1,
        kCancelOrder = 2,
        kModifyOrder = 3,

    }

}