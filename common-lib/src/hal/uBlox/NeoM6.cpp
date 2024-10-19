#include "hal/uBlox/NeoM6.hpp"

namespace common::hal
{
auto NeoM6::parse(const std::string& message) noexcept -> Position
{
    Position position;

    if(GPGGA(message, position))
    {
        position._type = (position._type | SupportType::GPGGA);
    }

    if(GPRMC(message, position))
    {
        position._type = (position._type | SupportType::GPRMC);
    }

    if(GPVTG(message, position))
    {
        position._type = (position._type | SupportType::GPVTG);
    }

    return position;
}

auto NeoM6::GPGGA(const std::string& message, Position& position) -> bool
{
    return true;
}

auto NeoM6::GPRMC(const std::string& message, Position& position) -> bool
{
    return true;
}

auto NeoM6::GPVTG(const std::string& message, Position& position) -> bool
{
    return true;
}
} // namespace common::hal