#pragma once

namespace pattern::lockable
{

template<typename DerivedClass>
class LockBase
{
public :
    auto Lock() -> void { reinterpret_cast<DerivedClass*>(this)->_lock(); }
    auto TryLock() -> bool { return reinterpret_cast<DerivedClass*>(this)->_trylock(); }
    auto Unlock() -> void { reinterpret_cast<DerivedClass*>(this)->_unlock(); }
};

} // namespace pattern::lockable