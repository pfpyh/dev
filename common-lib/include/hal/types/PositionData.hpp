#pragma once

#include "common/Observer.hpp"

namespace common::hal
{
struct PositionData
{
    uint32_t _type = 0x0;
    double _utc = 0.0;
    double _longitude = 0.0;
    double _latitude = 0.0;
    int32_t _altitude = 0;
};

class EventUpdatePositionData : public Subject<PositionData> {};
} // namespace hal