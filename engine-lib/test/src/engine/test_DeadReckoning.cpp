#include <gtest/gtest.h>

#include "engine/DeadReckoning.hpp"

namespace engine::test
{
TEST(DeadReckoningTest, Initialize)
{
    // given
    class MockImuSerial : public common::Serial
    {
    public :
        auto open([[maybe_unused]] const std::string& port,
                  [[maybe_unused]] const common::Baudrate baudRate,
                  [[maybe_unused]] const uint8_t mode) noexcept -> bool override
        {
            return true;
        }

        auto close() noexcept -> void override {}
        
        auto is_open() noexcept -> bool override { return true; }

        auto readline() noexcept -> std::string override 
        { 
            return "";
        }

        auto write([[maybe_unused]] const char* buffer, 
                   [[maybe_unused]] const size_t size) noexcept -> bool override
        {
            return true;
        }
    };

    class MockBodySerial : public common::Serial
    {
    public :
        auto open([[maybe_unused]] const std::string& port,
                  [[maybe_unused]] const common::Baudrate baudRate,
                  [[maybe_unused]] const uint8_t mode) noexcept -> bool override
        {
            return true;
        }

        auto close() noexcept -> void override {}
        
        auto is_open() noexcept -> bool override { return true; }

        auto readline() noexcept -> std::string override 
        { 
            return "";
        }

        auto write([[maybe_unused]] const char* buffer, 
                   [[maybe_unused]] const size_t size) noexcept -> bool override
        {
            return true;
        }
    };

    auto imuSerial = MockImuSerial::create();
    auto bodySerial = MockBodySerial::create();

    DeadReckoning engine;
    
    engine.initialize(imuSerial, bodySerial);

    // when

    // then
}
} // namespace engine::test