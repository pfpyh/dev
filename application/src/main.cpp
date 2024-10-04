#include "common-lib/communication/Serial.hpp"
#include "common-lib/logging/Logger.hpp"
#include "hal/Gnss.hpp"

#include "GnssReceiver.hpp"

int main()
{
    using namespace common;

    auto serial = Serial::create();
    if (!serial->open("COM7", Baudrate::_9600, SERIAL_READ | SERIAL_WRITE))
    {
        _ERROR_("Failed to open serial port");
        return -1;
    }

    GnssReceiver receiver;

    hal::Gnss gnss(serial);
    gnss.subscribe_update_position(std::make_shared<GnssReceiver>(receiver));
    auto future = gnss.run();   
    future.wait();

    return 0;
}