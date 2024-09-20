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

#pragma once

#include "common-lib/thread/Thread.hpp"
#include "common-lib/exception/Exception.hpp"
#include "common-lib/NonCopyable.hpp"

namespace common::thread
{
class Runnable : public NonCopyable
{
private :
    std::promise<void> _promise;
    bool _running = false;

public :
    auto run() -> std::future<void>
    {
        if(_running) throw exception::AlreadyRunningException();
        _running = true;

        auto t = Thread::create();
        t->start([this](){            
            while(_running) { work(); }
            _promise.set_value();
        });
        t->detach();

        return _promise.get_future();
    }
    inline auto stop() noexcept -> void { _running = false; }
    inline auto status() const noexcept -> bool { return _running; }

protected :
    virtual auto work() -> void = 0;
};

template <typename DataType>
class ActiveRunnable
{
private :
    std::promise<void> _promise;
    bool _running = false;

    std::mutex _notifyLock;
    std::condition_variable _cv;
    std::vector<DataType> _datas;

public :
    auto run() -> std::future<void>
    {
        if(_running) throw exception::AlreadyRunningException();
        _running = true;

        auto t = Thread::create();
        t->start([this](){            
            while(_running)
            {
                std::unique_lock<std::mutex> lock(_notifyLock);
                if(_datas.empty()) _cv.wait(lock);
                if(!_running) break;

                DataType data = _datas[0];
                _datas.erase(_datas.begin());
                lock.unlock();

                work(std::move(data));
            }
            _promise.set_value();
        });
        t->detach();

        return _promise.get_future();
    }

    auto notify(const DataType& data) noexcept -> void
    {
        std::unique_lock<std::mutex> lock(_notifyLock);
        _datas.push_back(data);
        _cv.notify_one();
    }

    auto notify(DataType&& data) noexcept -> void
    {
        std::unique_lock<std::mutex> lock(_notifyLock);
        _datas.push_back(std::move(data));
        _cv.notify_one();
    }

    auto stop() noexcept -> void
    {
        std::unique_lock<std::mutex> lock(_notifyLock);
        _running = false;
        _cv.notify_one();
    }

    inline auto status() const noexcept -> bool { return _running; }

protected :
    virtual auto work(DataType&& data) -> void = 0;
};
} // namespace common::thread
