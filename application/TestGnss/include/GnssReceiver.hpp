#pragma once

#include "common/Observer.hpp"
#include "hal/Gnss.hpp"

class GnssReceiver : public common::Observer<GnssReceiver, common::hal::PositionData>
{
private :
    common::hal::PositionData _last;

public :
    auto onEvent(common::hal::PositionData data) -> void override;
};