#pragma once

#include "hal/types/PositionData.hpp"

#include <string>

namespace common::hal
{
class NeoM6
{
public :
    class SupportType
    {
    public :
        enum type : uint32_t
        {
            GPGGA = 0x0001,
            GPRMC = 0x0002,
            GPVTG = 0x0004,
        };
    };

public :
    static auto parse(const std::string& message) noexcept -> PositionData;
};
} // common::hal