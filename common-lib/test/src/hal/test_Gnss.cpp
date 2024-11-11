#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "hal/Gnss.hpp"
#include "common/logging/Logger.hpp"

#include "mock_Serial.hpp"

namespace common::hal::test
{
TEST(test_Gnss, common_test)
{
    // given
    auto mockSerial = std::make_shared<mock::MockSerial>();
    EXPECT_CALL(*mockSerial, readline())
        .WillRepeatedly(testing::Return("$GPGGA,114455.532,3735.0079,N,12701.6446,E,1,03,7.9,48.8,M,19.6,M,0.0,0000*48"));

    auto gnss = Gnss::create(mockSerial);

    // when
    class Receiver : public Observer<Receiver, Position>
    {
    public :
        Position _last;
        bool _received = false;
        std::weak_ptr<Gnss> _gnss;

    public :
        Receiver(decltype(_gnss) gnss)
            : _gnss(gnss) {}

    public:
        auto onEvent(Position position) -> void override
        {
            _last = position;
            _received = true;
            _gnss.lock()->stop();
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