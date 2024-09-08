/*
MIT License

Copyright (c) 2024, Younghwan Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "common-lib/thread/Timer.hpp"
#include "common-lib/thread/Thread.hpp"
#include "common-lib/exception/Exception.hpp"

#include <thread>
#include <exception>

#include <iostream>

namespace common::thread
{
namespace detail
{
class TimerDetail final : public Timer
{
private :
    Function _func;
    Interval _interval;

public :
    TimerDetail(Function&& func,
                Interval interval) noexcept
        : _func(func)
        , _interval(interval) {}

public :
    auto start() -> std::shared_ptr<std::future<void>> override
    {
        if(_running) 
        { 
            throw exception::Exception(exception::AlreadyRunning); 
        }
        _running = true;

        auto promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future();
        auto thread = Thread::create();
        thread->start([this, promise = std::move(promise)](){
            while(true)
            {
                if(false == _running || false == _func()) { break; }
                std::this_thread::sleep_for(_interval);
            }
            promise->set_value();
        });
        thread->detach();
        return std::make_shared<std::future<void>>(std::move(future));
    }

    auto stop() noexcept -> void override
    {
        _running = false;
    }
};
} // namespace detail

auto Timer::__create(Function&& func, Interval interval) noexcept -> std::shared_ptr<Timer>
{
    return std::shared_ptr<Timer>(new detail::TimerDetail(std::forward<Function>(func), 
                                                          interval));
}
} // namespace common::thread