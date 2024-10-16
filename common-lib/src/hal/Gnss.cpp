#include "hal/Gnss.hpp"
#include "hal/uBlox/NeoM6.hpp"

namespace common::hal
{
Gnss::Gnss(std::shared_ptr<Serial> serial)
    : _serial(serial) {}

auto Gnss::subscribe_update_position(Observer<Position>* observer) -> void
{
    _updatePosition.regist(observer);
}

auto Gnss::unsubscribe_update_position(Observer<Position>* observer) -> void
{
    _updatePosition.unregist(observer);
}

auto Gnss::__work() -> void
{
    const std::string msg(_serial->readline());
    if(NeoM6::GPGGA(msg, _position))
    {
        _updatePosition.notify(_position);
    }
}
} // namespace common::hal