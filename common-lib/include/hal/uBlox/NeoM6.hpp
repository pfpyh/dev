#pragma once

#include "hal/types/Position.hpp"

#include <string>

namespace common::hal
{
class NeoM6
{
public :
    static auto GPGGA(const std::string& msg, Position& position) -> bool;
    static auto GPRMC(const std::string& msg, Position& position) -> bool;
    static auto GPVTG(const std::string& msg, Position& position) -> bool;
};
} // common::hal