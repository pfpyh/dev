#pragma once

#include "common-lib/Observer.hpp"
#include "common-lib/thread/Runnable.hpp"
#include "common-lib/Factory.hpp"
#include "common-lib/communication/Serial.hpp"

#include "hal/types/Position.hpp"

namespace common::hal
{
class Gnss : public Runnable
           , public Factory<Gnss>
{
    friend class Factory<Gnss>;

private :
    std::shared_ptr<Serial> _serial;
    EventUpdatePosition _updatePosition;
    Position _position;

public :
    Gnss(std::shared_ptr<Serial> serial);

public :
    auto subscribe_update_position(std::shared_ptr<BaseObserver<Position>> observer) -> void;
    auto unsubscribe_update_position(std::shared_ptr<BaseObserver<Position>> observer) -> void;

private :
    auto __work() -> void override;
    static auto __create(std::shared_ptr<Serial> serial) -> std::shared_ptr<Gnss>;
};
} // namespace common::hal