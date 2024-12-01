#pragma once

#include "CommonHeader.hpp"
#include "hal/types/ImuData.hpp"

#include <tuple>
#include <string>

namespace common::hal
{
class COMMON_LIB_API MPU6050
{
public :
    static auto parse(const std::string& message) noexcept -> std::tuple<AccData, GyroData>;
};
} // namespace common::hal