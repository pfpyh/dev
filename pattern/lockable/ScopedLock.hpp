#pragma once

#include <memory>

namespace pattern::lockable
{

template<typename LockableClass>
class ScopedLock
{
private :
    std::shared_ptr<LockableClass> __manager;

public :
    ScopedLock( LockableClass & lockable )
    {
        __manager = std::shared_ptr<LockableClass>( &lockable, 
                                                    []( LockableClass * ptr ) {
            ptr->Unlock();
        } );
        lockable.Lock();
    };
};

} // pattern::lockable