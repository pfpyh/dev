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

#include "common-lib/thread/Thread.hpp"

#include <thread>
#include <functional>

namespace common::thread
{
namespace detail
{
class ThreadDetail final : public Thread
{
private :
    std::thread _thread;
    std::promise<void> _promise;

public :
    ~ThreadDetail() noexcept final = default;

public :
    auto start(std::function<void()>&& func) noexcept -> std::future<void> override
    {
        auto future = _promise.get_future();
        _thread = std::thread([this, work = std::move(func)]() mutable 
        {
            work();
            _promise.set_value();
        });
        return future;
    } 

    auto detach() noexcept -> void override
    {
        _thread.detach();
    }

    auto join() noexcept -> void override
    {
        if(_thread.joinable())
        {
            _thread.join();
        }
    }    
};
} // namespace detail

auto Thread::__create() noexcept -> std::shared_ptr<Thread>
{
    return std::shared_ptr<Thread>(new detail::ThreadDetail, [](detail::ThreadDetail* obj){
        obj->join();
    });
}
} // namespace common::thread