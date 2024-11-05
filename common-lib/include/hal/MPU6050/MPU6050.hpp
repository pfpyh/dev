#pragma once

#include "hal/types/IMUData.hpp"

#include <tuple>
#include <string>

namespace common::hal
{
class MPU6050
{
public :
    static auto parse(const std::string& message) noexcept -> std::tuple<AccData, GyroData>;
};
} // namespace common::hal