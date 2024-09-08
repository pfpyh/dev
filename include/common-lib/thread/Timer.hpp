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

#include "common-lib/NonCopyable.hpp"
#include "common-lib/Factory.hpp"

#include <functional>
#include <memory>
#include <chrono>
#include <future>

namespace common::thread
{
class Timer : public NonCopyable, 
              public Factory<Timer, std::function<bool()>, std::chrono::microseconds>
{
    friend class Factory<Timer, std::function<bool()>, std::chrono::microseconds>;

public :
    using Function = std::function<bool()>;
    using Interval = std::chrono::microseconds;

protected :
    bool _running = false;

public :
    static auto __create(Function&& func, Interval interval) noexcept -> std::shared_ptr<Timer>;

public :
    virtual auto start() -> std::shared_ptr<std::future<void>> = 0;
    virtual auto stop() noexcept -> void = 0;
    inline auto running() noexcept -> bool { return _running; }
};
} // namespace common::thread