#pragma once

#include <thread>
#include <memory>
#include <future>

namespace pattern::thread
{

template<typename DerivedClass, typename ReturnType>
class ThreadBase
{
protected :
    std::shared_ptr<std::promise<ReturnType>> _promise = nullptr;
    std::function<ReturnType()> _func;

public :
    ThreadBase( decltype(_func) func )
        : _func( func ) {};

public :
    auto Run() -> decltype(auto)
    {
        _promise = std::make_shared<std::promise<ReturnType>>();
        return reinterpret_cast<DerivedClass*>(this)->_post();
    };
};

template<typename ReturnType>
class AsyncThread : public ThreadBase<AsyncThread<ReturnType>, ReturnType>
{
public :
    AsyncThread( decltype(_func) func )
        : ThreadBase<AsyncThread<ReturnType>, ReturnType>( func ) {};

private :
    auto _post() -> std::future<ReturnType>
    {
        std::thread t( [this]() { _promise->set_value( _func() ); } );
        t.detach();

        return _promise->get_future(); 
    };
    friend ThreadBase<AsyncThread<ReturnType>, ReturnType>;
};

class SyncThread : public ThreadBase<SyncThread, void>
{
public :
    SyncThread( decltype(_func) func )
        : ThreadBase<SyncThread, void>( func ) {};

private :
    auto _post() -> void
    {
        std::thread t( [this]() {
            _func();
            _promise->set_value();
        } );
        t.detach();

        auto future = _promise->get_future();
        future.wait();
    };
    friend ThreadBase<SyncThread, void>;
};

} // namespace pattern::thread