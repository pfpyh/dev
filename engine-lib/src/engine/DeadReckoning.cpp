#include "engine/DeadReckoning.hpp"
#include "common/logging/Logger.hpp"

namespace engine
{
namespace detail
{
auto DeadReckoningThread::__work() -> void
{

}
} // namespace detail

auto DeadReckoning::initialize(std::shared_ptr<common::Serial> imuSerial,
                               std::shared_ptr<common::Serial> bodySe;rial) -> bool
{
    _imu = common::hal::Imu::create(imuSerial);
    _futures.push_back(std::move(_imu->run()));

    _body = common::hal::Body::create(bodySerial);
    _futures.push_back(std::move(_body->run()));

    // _futures.push_back(std::move(_thread.run()));
    return false;
}

auto DeadReckoning::finalize() -> void
{
    // _thread.stop();
    _body->stop();
    _imu->stop();
    
    for(auto& future : _futures) { future.wait(); }
}

// auto DeadReckoning::onDirectionUpdate(const double direction) -> void
// {
//     _INFO_("DIR[%f]", direction);
// }

// auto DeadReckoning::onSpeedUpdate(const double speed) -> void
// {
//     _INFO_("SPEED[%f]", speed);
// }
} // engine