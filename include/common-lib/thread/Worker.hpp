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

#include <functional>
#include <memory>
#include <thread>
#include <future>

#include <iostream>

namespace common::thread
{
template <typename ReturnType>
auto run(std::function<ReturnType()> work) -> std::shared_ptr<std::future<ReturnType>>
{
    std::shared_ptr<std::promise<ReturnType>> promise =
        std::make_shared<std::promise<ReturnType>>();

    std::thread t([work, promise](){
        promise->set_value(work());
    });
    t.detach();

    std::shared_ptr<std::future<ReturnType>> future = 
        std::make_shared<std::future<ReturnType>>(promise->get_future());
    return future;
}

template <typename Derived>
class SingleWorker
{
private :
    std::shared_ptr<std::promise<void>> _promise;
    std::shared_ptr<std::future<void>> _future;
    bool _running = false;

public :
    auto run() -> std::shared_ptr<std::future<void>>
    {
        if(_running) // already runing
        {
            stop();
            _future->wait();
        }

        _promise = std::make_shared<std::promise<void>>();
        _future = std::make_shared<std::future<void>>(_promise->get_future());
        _running = true;

        std::thread t([this](){
            while(_running)
            {
                static_cast<Proxy*>(this)->__work();
            }
            _promise->set_value();
        });
        t.detach();

        return _future;
    }

    auto stop() -> void { _running = false; }
    auto wait_until_stop() -> void { _future->wait(); }
    auto status() const -> bool { return _running; }

private :
    class Proxy : public Derived
    {
        friend auto SingleWorker::run() -> std::shared_ptr<std::future<void>>;
    };
};

template <typename Derived, typename DataType>
class SingleActiveWorker
{
private :
    std::shared_ptr<std::promise<void>> _promise;
    std::shared_ptr<std::future<void>> _future;
    bool _running = false;

    std::mutex _notifyLock;
    std::condition_variable _cv;
    std::vector<DataType> _datas;

public :
    auto run() -> std::shared_ptr<std::future<void>>
    {
        if(_running) // already runing
        {
            stop();
            _future->wait();
        }

        _promise = std::make_shared<std::promise<void>>();
        _future = std::make_shared<std::future<void>>(_promise->get_future());
        _running = true;

        std::thread t([this](){
            while(_running)
            {
                std::unique_lock<std::mutex> lock(_notifyLock);
                _cv.wait(lock);
                if(_datas.empty()) break;

                DataType data = _datas[0];
                _datas.erase(_datas.begin());
                lock.unlock();

                static_cast<Proxy*>(this)->__work(std::move(data));
            }
            _promise->set_value(); 
        });
        t.detach();

        return _future;
    }

    auto notify(DataType&& data) -> void
    {
        std::unique_lock<std::mutex> lock(_notifyLock);
        _datas.push_back(std::move(data));
        _cv.notify_one();
    }

    auto notify(const DataType& data) -> void
    {
        std::unique_lock<std::mutex> lock(_notifyLock);
        _datas.push_back(data);
        _cv.notify_one();
    }

    auto stop() -> void
    {
        std::unique_lock<std::mutex> lock(_notifyLock);
        _running = false;
        _cv.notify_one();
    }

    auto wait_until_stop() -> void
    {
        _future->wait();
    }

    auto status() -> bool
    {
        return _running;
    }

private :
    class Proxy : public Derived
    {
        friend auto SingleActiveWorker::run() -> std::shared_ptr<std::future<void>>;
    };
};
} // namespace common::thread
