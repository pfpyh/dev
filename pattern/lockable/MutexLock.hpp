#pragma once

#include "LockBase.hpp"
#include <mutex>

namespace pattern::lockable
{

class MutexLock : public LockBase<MutexLock>
{
private :
    std::mutex lock;

    auto _Lock() -> void
    {
        lock.lock();
    };

    auto _TryLock() -> bool
    {
        return lock.try_lock();
    };

    auto _Unlock() -> void
    {
        lock.unlock();
    };

    friend LockBase<MutexLock>;
};

} // namespace pattern::lockable