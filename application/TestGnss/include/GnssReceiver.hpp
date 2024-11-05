#pragma once

#include "common/Observer.hpp"
#include "hal/Gnss.hpp"

class GnssReceiver : public common::Observer<GnssReceiver, common::hal::Position>
{
private :
    common::hal::Position _last;

public :
    auto onEvent(common::hal::Position position) -> void override;
};