#include "hal/MPU6050/MPU6050DefaultObserver.hpp"

#include "common/logging/Logger.hpp"

namespace common::hal
{
MPU6050DefaultObserver::MPU6050DefaultObserver(std::function<void(const double)>&& callback) 
    : _callback(std::forward<std::function<void(const double)>>(callback)) {}

auto MPU6050DefaultObserver::onEvent(common::hal::ImuData data) -> void
{
    auto now = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _prevRcvTime);
    _prevRcvTime = now;

    constexpr double dt = 0.01; // 100Hz
    _direction += data._z * dt;
    while (_direction > 180.0) { _direction -= 360.0; }
    while (_direction < -180.0) { _direction += 360.0; }

    _callback(_direction);

    _INFO_("IMU[%f, %f, %f] DIR[%f] (%lld ms)", data._x, data._y, data._z, _direction, duration.count());
}
} // namespace common::hal