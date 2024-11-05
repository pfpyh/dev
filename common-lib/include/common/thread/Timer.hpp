#pragma once

#include "common/NonCopyable.hpp"
#include "common/Factory.hpp"

#include <functional>
#include <memory>
#include <chrono>
#include <future>

namespace common
{
/**
 * @brief Represents a timer that can execute a given function at a specified interval.
 *
 * This class represents a timer that can execute a given function at a specified interval.
 * It provides a way to create and manage timers, allowing for periodic execution of tasks.
 * The task can be stopped by calling stop(), and the timer started by start() will be stopped.
 * If the task is stopped, running() will return false.
 *
 * @note A derived class must implement the pure virtual functions start() and stop() to execute a task.
 */
class Timer : public NonCopyable, 
              public UniqueFactory<Timer>
{
    friend class UniqueFactory<Timer>;

public :
    using Function = std::function<bool()>;
    using Interval = std::chrono::microseconds;

public :
    ~Timer() noexcept = default;

private :
    /**
     * @brief Create a new timer with the given function and interval.
     *
     * @param func The function to execute at the specified interval.
     * @param interval The interval at which the function should be executed.
     *
     * @return A new timer that can execute the given function at the specified interval.
     */
    static auto __create(Function&& func, Interval interval) noexcept -> std::unique_ptr<Timer>;

public :
    /**
     * @brief Create a new timer that will execute the given function asynchronously at the specified interval.
     *
     * @param func The function to execute at the specified interval.
     * @param interval The interval at which the function should be executed.
     *
     * @return A future that can be used to wait for the timer to finish.
     */
    static auto async(Function&& func, Interval interval) noexcept -> std::future<void>;

public :
    /**
     * @brief Start the timer and execute the given function at the specified interval.
     *
     * @return A future that can be used to wait for the timer to finish.
     * @throw common::exception::AlreadyRunningException if start() is called multiple times.
     */
    virtual auto start() -> std::future<void> = 0;

    /**
     * @brief Stop the timer and stop executing the given function.
     */
    virtual auto stop() noexcept -> void = 0;

    /**
     * @brief Check if the timer is running.
     *
     * @return true if the timer is running, false otherwise.
     */
    virtual auto running() noexcept -> bool = 0;
};
} // namespace common