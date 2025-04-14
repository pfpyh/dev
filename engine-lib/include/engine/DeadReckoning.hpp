#pragma once

#include "common/thread/Runnable.hpp"

#include "hal/imu/Imu.hpp"
#include "hal/imu/MPU6050.hpp"

#include "hal/body/Body.hpp"
#include "hal/body/Default.hpp"

namespace engine
{
namespace detail
{
class DeadReckoningThread : public common::Runnable
{
private :
    auto __work() -> void override;
};
} // namespace detail

class DeadReckoning // : public ::common::hal::MPU6050::DefaultObserver
                    // , private common::hal::default::DefaultObserver
{
private :
    std::shared_ptr<common::hal::Imu> _imu;
    std::shared_ptr<common::hal::Body> _body;

    // detail::DeadReckoningThread _thread;
    std::vector<std::future<void>> _futures;

public :
    auto initialize(std::shared_ptr<common::Serial> imuSerial,
                    std::shared_ptr<common::Serial> bodySerial) -> bool;
    auto finalize() -> void;

public :
    // auto onDirectionUpdate(const double direction) -> void override;
    // auto onSpeedUpdate(const double speed) -> void override;
};
} // namespace engine