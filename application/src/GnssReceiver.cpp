#include "GnssReceiver.hpp"

#include "common-lib/logging/Logger.hpp"

auto GnssReceiver::onEvent(common::hal::Position position) -> void
{
    _INFO_("Position: %f, %f, %d", position._longitude, position._latitude, position._altitude);
}