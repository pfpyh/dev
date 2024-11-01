#pragma once

#include "common/Observer.hpp"
#include "common/thread/Runnable.hpp"
#include "common/Factory.hpp"
#include "common/communication/Serial.hpp"

#include "hal/types/ImuData.hpp"

namespace common::hal
{
class Imu : public Runnable
          , public Factory<Imu>
{
    friend class Factory<Imu>;

private :
    std::shared_ptr<Serial> _serial;
    EventUpdateImuData _updateImuData;
    ImuData _data;

public :
    Imu(std::shared_ptr<Serial> serial);

public :
    auto subscribe_update_imuData(std::shared_ptr<BaseObserver<ImuData>> observer) -> void;
    auto unsubscribe_update_imuData(std::shared_ptr<BaseObserver<ImuData>> observer) -> void;

private :
    auto __work() -> void override;
    static auto __create(std::shared_ptr<Serial> serial) -> std::shared_ptr<Imu>;
};
} // namespace common::hal