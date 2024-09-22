#include <gtest/gtest.h>

#include "common-lib/thread/Timer.hpp"
#include "common-lib/logging/Logger.hpp"

namespace common::test
{
TEST(test_Timer, start)
{
    // given
    uint8_t count = 0;
    auto timer = Timer::create([&count]() -> bool{
        ++count;
        if (count == 10) return false;
        return true;
    }, std::chrono::milliseconds(10));

    // when
    auto future = timer->start();
    future.wait();

    // then
    ASSERT_EQ(count, 10);
}

TEST(test_Timer, stop)
{
    // given
    uint8_t count = 0;
    auto timer = Timer::create([&count]() -> bool{
        ++count;
        if (count == 255) return false;
        return true;
    }, std::chrono::milliseconds(10));

    // when
    auto future = timer->start();
    timer->stop();
    future.wait();

    // then
    ASSERT_NE(count, 255);
}
} // namespace common::test