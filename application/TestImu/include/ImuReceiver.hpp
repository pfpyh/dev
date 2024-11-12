#pragma once

#include "common/Observer.hpp"
#include "hal/types/ImuData.hpp"

#include <chrono>

class ImuReceiver : public common::Observer<ImuReceiver, common::hal::ImuData>
{
private :
    common::hal::ImuData _last;
    std::chrono::time_point<std::chrono::steady_clock> _prevRcvTime = std::chrono::steady_clock::now();

    double _direction = 0.0;

public :
    auto onEvent(common::hal::ImuData data) -> void override;
};