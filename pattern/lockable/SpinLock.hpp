#pragma once

#include "LockBase.hpp"
#include <atomic>

namespace pattern::lockable
{

class SpinLock : public LockBase<SpinLock>
{
private :
    std::atomic_flag lock = ATOMIC_FLAG_INIT;

    auto _lock() -> void
    {
        while( lock.test_and_set( std::memory_order_acquire ) ) {};
    };

    auto _trylock() -> bool
    {
        return !lock.test_and_set( std::memory_order_acquire );
    };

    auto _unlock() -> void
    {
        lock.clear( std::memory_order_release );
    };

    friend LockBase<SpinLock>;
};

} // namespace pattern::lockable