#include "hal/Gnss.hpp"
#include "hal/uBlox/NeoM6.hpp"

namespace common::hal
{
Gnss::Gnss(std::shared_ptr<Serial> serial)
    : _serial(serial) {}

auto Gnss::subscribe_update_position(std::shared_ptr<BaseObserver<Position>> observer) -> void
{
    _updatePosition.regist(std::move(observer));
}

auto Gnss::unsubscribe_update_position(std::shared_ptr<BaseObserver<Position>> observer) -> void
{
    _updatePosition.unregist(std::move(observer));
}

auto Gnss::__work() -> void
{
    const std::string msg(_serial->readline());
    auto position = NeoM6::parse(msg);

    if(position._type & NeoM6::SupportType::GPGGA)
    {
        _updatePosition.notify(_position);
    }
}

auto Gnss::__create(std::shared_ptr<Serial> serial) -> std::shared_ptr<Gnss>
{
    return std::make_shared<Gnss>(serial);
}
} // namespace common::hal