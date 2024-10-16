#pragma once

#include "common-lib/Observer.hpp"
#include "common-lib/thread/Runnable.hpp"
#include "common-lib/communication/Serial.hpp"

#include "hal/types/Position.hpp"

namespace common::hal
{
class Gnss : public Runnable
{
private :
    std::shared_ptr<Serial> _serial;
    EventUpdatePosition _updatePosition;
    Position _position;

public :
    Gnss(std::shared_ptr<Serial> serial);

public :
    auto subscribe_update_position(Observer<Position>* observer) -> void;
    auto unsubscribe_update_position(Observer<Position>* observer) -> void;

private :
    auto __work() -> void override;
};
} // namespace common::hal