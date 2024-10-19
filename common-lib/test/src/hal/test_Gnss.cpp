#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "hal/Gnss.hpp"
#include "common-lib/logging/Logger.hpp"

namespace common::hal::test
{
TEST(test_Gnss, common_test)
{
    // given
    class MockSerial : public Serial
    {
    public:
        MOCK_METHOD(bool, open, (const std::string&, const Baudrate::type, const uint8_t), (override, noexcept));
        MOCK_METHOD(void, close, (), (override, noexcept));
        MOCK_METHOD(bool, is_open, (), (override, noexcept));
        MOCK_METHOD(std::string, readline, (), (override, noexcept));
        MOCK_METHOD(bool, write, (const char*, const size_t), (override, noexcept));
    };

    auto mockSerial = MockSerial::create();
    auto gnss = Gnss::create(mockSerial);

    // when
    class Receiver : public Observer<Receiver, Position>
    {
    public :
        Position _last;
        bool _received = false;
        std::shared_ptr<Gnss> _gnss;

    public :
        Receiver(decltype(_gnss) gnss)
            : _gnss(gnss) {}

    public:
        auto onEvent(Position position) -> void override
        {
            _last = position;
            _received = true;
            _gnss->stop();
        }
    };

    auto receiver = Receiver::create(gnss);
    gnss->subscribe_update_position(receiver);
    auto future = gnss->run();

    // then
    future.wait();
    ASSERT_TRUE(receiver->_received);
}
} // namespace common::hal::test