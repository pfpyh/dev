#pragma once

#include "common/Observer.hpp"
#include "common/thread/Runnable.hpp"
#include "common/Factory.hpp"
#include "common/communication/Serial.hpp"

#include "hal/types/PositionData.hpp"

namespace common::hal
{
class Gnss : public Runnable
           , public Factory<Gnss>
{
    friend class Factory<Gnss>;

private :
    std::shared_ptr<Serial> _serial;
    EventUpdatePositionData _updatePosition;
    PositionData _position;

public :
    Gnss(std::shared_ptr<Serial> serial);

public :
    auto subscribe_update_position(std::shared_ptr<BaseObserver<PositionData>> observer) -> void;
    auto unsubscribe_update_position(std::shared_ptr<BaseObserver<PositionData>> observer) -> void;

private :
    auto __work() -> void override;
    static auto __create(std::shared_ptr<Serial> serial) -> std::shared_ptr<Gnss>;
};
} // namespace common::hal