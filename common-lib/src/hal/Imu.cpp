#include "hal/Imu.hpp"
#include "hal/MPU6050/MPU6050.hpp"

#include "common/logging/Logger.hpp"
#include "math/constant.hpp"

namespace common::hal
{
Imu::Imu(std::shared_ptr<Serial> serial)
    : _serial(serial) {}

auto Imu::subscribe_update_imuData(std::shared_ptr<BaseObserver<ImuData>> observer) -> void
{
    _updateImuData.regist(std::move(observer));
}

auto Imu::unsubscribe_update_imuData(std::shared_ptr<BaseObserver<ImuData>> observer) -> void
{
    _updateImuData.unregist(std::move(observer));
}

auto Imu::__work() -> void
{
    const std::string msg(_serial->readline());
    const auto [acc, gyr] = MPU6050::parse(msg);

    /******************************************************************************* */
    /* Complimentary Filter */
    /******************************************************************************* */
    auto roll = atan2(acc._y, acc._z) * math::DEG2RAD;
    auto pitch = atan(-acc._x / sqrt(acc._y * acc._y + acc._z * acc._z)) * math::DEG2RAD;

    constexpr double ALPHA = 0.98;
    constexpr double dt = 0.01;

    _data._x = (1 - ALPHA) * (_data._x + gyr._x * dt) + ALPHA * roll;
    _data._y = (1 - ALPHA) * (_data._y + gyr._y * dt) + ALPHA * roll;
    /******************************************************************************* */

    _updateImuData.notify(_data);
}

auto Imu::__create(std::shared_ptr<Serial> serial) -> std::shared_ptr<Imu>
{
    return std::make_shared<Imu>(serial);
}
} // namespace common::hal