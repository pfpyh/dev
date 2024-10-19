#pragma once

#include "common-lib/Observer.hpp"

namespace common::hal
{
struct Position
{
    uint32_t _type = 0x0;
    int32_t _longitude = 0;
    int32_t _latitude = 0;
    int32_t _altitude = 0;
};

class EventUpdatePosition : public Subject<Position>
{

};
} // namespace hal