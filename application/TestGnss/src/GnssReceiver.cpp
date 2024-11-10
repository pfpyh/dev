#include "GnssReceiver.hpp"

#include "common/logging/Logger.hpp"

auto GnssReceiver::onEvent(common::hal::PositionData data) -> void
{
    _INFO_("Position: _utc:(%f) _longitude:(%f), _latitude:(%f), _altitude:(%d)",
           data._utc, data._longitude, data._latitude, data._altitude);
}