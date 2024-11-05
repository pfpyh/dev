#include "hal/MPU6050/MPU6050.hpp"

#include <sstream>

namespace common::hal
{
auto MPU6050::parse(const std::string& message) noexcept -> std::tuple<AccData, GyroData>
{
    std::vector<int32_t> values;
    std::string token;
    std::istringstream iss(message);
    while(std::getline(iss, token, ' '))
    {
        const auto value = std::stoi(token);
        values.push_back(value);
    }

    if(values.size() != 6)
    {
        return {AccData(), GyroData()};
    }

    AccData accData;
    accData._x = static_cast<double>(values[0] / 16384);
    accData._y = static_cast<double>(values[1] / 16384);
    accData._z = static_cast<double>(values[2] / 16384);

    GyroData gyroData;
    gyroData._x = static_cast<double>(values[3] / 131);
    gyroData._y = static_cast<double>(values[4] / 131);
    gyroData._z = static_cast<double>(values[5] / 131);

    return {accData, gyroData};
}
} // namespace common::hal