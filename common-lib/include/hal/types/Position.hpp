#pragma once

#include "common/Observer.hpp"

namespace common::hal
{
struct Position
{
    uint32_t _type = 0x0;
    double _longitude = 0;
    double _latitude = 0;
    int32_t _altitude = 0;
};

class EventUpdatePosition : public Subject<Position>
{

};
} // namespace hal