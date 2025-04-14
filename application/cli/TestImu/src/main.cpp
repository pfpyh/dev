#include "common/communication/Serial.hpp"
#include "common/logging/Logger.hpp"

#include "hal/imu/Imu.hpp"
#include "hal/imu/MPU6050.hpp"

class ImuReceiver : public ::common::hal::MPU6050::DefaultObserver
{
public :
    auto onDirectionUpdate(const double direction) -> void override 
    { 
        _INFO_("IMU direction: %f", direction); 
    }
};

int main()
{
    using namespace common;

    auto serial = Serial::create();
    if (!serial->open("COM5", Baudrate::_38400, SERIAL_READ))
    {
        _ERROR_("Failed to open serial port");
        return -1;
    }

    ImuReceiver receiver;

    auto imu = hal::Imu::create(serial);
    imu->subscribe_update_imuData(std::make_shared<ImuReceiver>(receiver));
    auto future = imu->run();
    future.wait();

    return 0;
}