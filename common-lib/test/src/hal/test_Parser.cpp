#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "hal/Parser.hpp"

namespace common::hal::test
{
TEST(test_Parser, common_test)
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

    auto mockSerial = std::make_shared<MockSerial>();
    auto parser = Parser::create(mockSerial);

    // when
    class Receiver : public Observer<Position>
    {
    public :
        Position _last;

    public:
        auto onEvent(Position position) -> void override
        {
            _last = position;
        }
    };
    Receiver receiver;
    parser->subscribe_update_position(&receiver);

    // then
}
} // namespace common::hal::test