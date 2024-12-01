#include "common/communication/Serial.hpp"

#include "hal/types/ImuData.hpp"
#include "hal/MPU6050/MPU6050DefaultObserver.hpp"
#include "hal/Imu.hpp"

#include <chrono>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace common;

class PY_IMU
{
private :
    std::shared_ptr<Serial> _serial;
    std::shared_ptr<hal::Imu> _imu;

    uint8_t _sequence = 0;
    std::unordered_map<uint8_t, std::shared_ptr<hal::MPU6050DefaultObserver>> _observers;

public :
    auto initialize(const char* const port, const uint32_t baudrate, const uint8_t mode) -> bool
    {
        _serial = Serial::create();
        if(!_serial->open(port, static_cast<Baudrate::type>(baudrate), mode)) 
        { 
            return false; 
        }
        _imu = std::make_shared<hal::Imu>(_serial);
        return true;
    }

    auto finalize() -> void
    {
        if(_serial) 
        { 
            _serial->close();
        }
    }

    auto subscribe_update_imuData(std::function<void(const double direction)>&& callback) -> uint8_t
    {
        auto observer = std::make_shared<hal::MPU6050DefaultObserver>(
            std::forward<std::function<void(const double)>>(callback)
        );
        _imu->subscribe_update_imuData(observer);
        ++_sequence;
        _observers[_sequence] = observer;
        return _sequence;
    }

    auto unsubscribe_update_imuData(uint8_t sequnece) -> void
    {
        auto itor = _observers.find(sequnece);
        if(itor != _observers.end()) 
        { 
            _imu->unsubscribe_update_imuData(itor->second);
            _observers.erase(itor); 
        }
    }
};

PYBIND11_MODULE(py_common_lib, m) 
{
    m.doc() = "common-lib for python";

    py::class_<PY_IMU>(m, "Imu")
        .def(py::init())
        .def("initialize", 
             &PY_IMU::initialize,
             "Initialize IMU with Serial port",
             py::arg("port"), py::arg("baudrate"), py::arg("mode"))
        .def("finalize", 
             &PY_IMU::finalize,
             "Close Serial port")
        .def("subscribe_update_imuData", 
             &PY_IMU::subscribe_update_imuData,
             "Subscribe IMU data",
             py::arg("callback"))
        .def("unsubscribe_update_imuData", 
             &PY_IMU::unsubscribe_update_imuData,
             "Ubsubscribe IMU data",
             py::arg("sequence"));
}