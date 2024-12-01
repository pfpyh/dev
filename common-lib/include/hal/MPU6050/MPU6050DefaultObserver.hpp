#pragma once

#include "CommonHeader.hpp"
#include "common/Observer.hpp"

#include "hal/types/ImuData.hpp"

#include <chrono>
#include <functional>

namespace common::hal
{
class COMMON_LIB_API MPU6050DefaultObserver : public Observer<MPU6050DefaultObserver, ImuData>
{
private :
    common::hal::ImuData _last;
    std::chrono::time_point<std::chrono::steady_clock> _prevRcvTime = std::chrono::steady_clock::now();
    double _direction = 0.0;

    std::function<void(const double)> _callback;

public :
    MPU6050DefaultObserver(std::function<void(const double)>&& callback);

private :
    auto onEvent(common::hal::ImuData data) -> void override;
};
} // namespace common::hal