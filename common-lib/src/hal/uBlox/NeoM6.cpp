#include "hal/uBlox/NeoM6.hpp"

namespace common::hal
{
auto NeoM6::GPGGA(const std::string& msg, Position& position) -> bool
{
    return false;
}

auto NeoM6::GPRMC(const std::string& msg, Position& position) -> bool
{
    return false;
}

auto NeoM6::GPVTG(const std::string& msg, Position& position) -> bool
{
    return false;
}
} // namespace common::hal