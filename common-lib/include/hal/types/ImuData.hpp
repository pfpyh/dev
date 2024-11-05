#pragma once

#include "common/Observer.hpp"

namespace common::hal
{
struct ImuRawData
{
    double _x = 0.0;
    double _y = 0.0;
    double _z = 0.0;
};

using GyroData = ImuRawData; /* unit = g */
using AccData = ImuRawData; /* unit = deg/s */
using ImuData = ImuRawData;

class EventUpdateImuData : public Subject<ImuData> {};
} // namespace common::hal