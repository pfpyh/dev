#pragma once

#include "common-lib/Factory.hpp"
#include "common-lib/Observer.hpp"
#include "common-lib/thread/Runnable.hpp"
#include "common-lib/communication/Serial.hpp"

namespace common::hal
{
struct Position
{
    int32_t _lon = 0;
    int32_t _lat = 0;
};

class EventUpdatePosition : public Subject<Position>
{

};

class Parser : public Factory<Parser, std::shared_ptr<Serial>>
             , public Runnable
{
    friend class Factory<Parser, std::shared_ptr<Serial>>;

private :
    EventUpdatePosition _updatePosition;

public :
    auto subscribe_update_position(Observer<Position>* observer) -> void;
    auto unsubscribe_update_position(Observer<Position>* observer) -> void;

private :
    auto __work() -> void override;
    static auto __create(std::shared_ptr<Serial> serial) noexcept -> std::shared_ptr<Parser>;
};
} // namespace common::hal