#include <gtest/gtest.h>

#include "common-lib/thread/Thread.hpp"

namespace common::thread::test
{
TEST(ThreadTest, create)
{
    // given
    bool value = false;
    auto t = Thread::create();

    // when
    auto future = t->start([&value](){
        value = true;
    });
    t->detach();
    future.wait();

    // then
    ASSERT_TRUE(value);
}

TEST(ThreadTest, priority)
{
    // given
    std::promise<void> promise;
    auto future = promise.get_future();
    auto t = Thread::create();

#if defined(WIN32)
    const Thread::Priority origin = Thread::Policies::DEFAULT;
    const Thread::Priority setting = Thread::Policies::TIME_CRITICAL;
#elif deinfed(LINUX)

#endif

    // when
    auto threadFuture = t->start([&future](){
        future.wait();
    });

    auto before = t->get_priority();
    const bool rtn = t->set_priority(setting);
    auto after = t->get_priority();

    // then
    ASSERT_TRUE(rtn);
    EXPECT_EQ(before, origin);
    EXPECT_EQ(after, setting);

    promise.set_value();   
    threadFuture.wait();
}

TEST(ThreadTest, task)
{
    // given
    bool value = false;

    // when
    auto future = Thread::task([&value](){
        value = true; 
    });
    future.wait();

    // then
    ASSERT_TRUE(value);
}
} // namespace common::thread::test