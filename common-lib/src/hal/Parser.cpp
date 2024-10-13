#include "hal/Parser.hpp"

namespace common::hal
{
auto Parser::subscribe_update_position(Observer<Position>* observer) -> void
{
    _updatePosition.regist(observer);
}

auto Parser::unsubscribe_update_position(Observer<Position>* observer) -> void
{
    _updatePosition.unregist(observer);
}

auto Parser::__work() -> void
{
    Position pos;
    pos._lat = 10;
    pos._lon = 10;

    _updatePosition.notify(pos);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

auto Parser::__create(std::shared_ptr<Serial> serial) noexcept -> std::shared_ptr<Parser>
{
    return nullptr;
}
} // namespace common::hal