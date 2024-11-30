#include "common/communication/Serial.hpp"
#include "common/logging/Logger.hpp"
#include "hal/Imu.hpp"

#include "ImuReceiver.hpp"

int main()
{
    using namespace common;

    auto serial = Serial::create();
    if (!serial->open("COM3", Baudrate::_38400, SERIAL_READ))
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