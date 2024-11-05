#pragma once

#include "common/NonCopyable.hpp"
#include "common/Factory.hpp"

#include <future>
#if defined(WIN32)
#include <windows.h>
#elif defined(LINUX)

#endif

namespace common
{
/**
 * @brief Represents a thread that can be executed independently.
 * 
 * This class provides a way to create and manage threads, allowing for concurrent execution of tasks.
 * It offers methods for starting, detaching, joining, and setting the priority of the thread.
 * 
 * @note Detailed implementation can be different based on sysytem.
 */
class Thread : public NonCopyable, public Factory<Thread>
{
    friend class Factory<Thread>;

public :
#if defined(WIN32)
    class Policies
    {
    public :
        enum type : int32_t
        { 
            DEFAULT = THREAD_PRIORITY_NORMAL,
            ABOVE_NORMAL = THREAD_PRIORITY_ABOVE_NORMAL,
            BELOW_NORMAL = THREAD_PRIORITY_BELOW_NORMAL,
            HIGHEST = THREAD_PRIORITY_HIGHEST,
            IDLE = THREAD_PRIORITY_IDLE,
            LOWEST = THREAD_PRIORITY_LOWEST,
            NORMAL = THREAD_PRIORITY_NORMAL,
            TIME_CRITICAL = THREAD_PRIORITY_TIME_CRITICAL,
        };
    };

    using Priority = Policies::type;
#elif defined(LINUX)    
    class Policies
    {
    public :
        enum type : uint32_t
        {
            DEFAULT = SCHED_OTHER,
            OTHER = SCHED_OTHER,
            FIFO = SCHED_FIFO,
            RR = SCHED_RR,
            BATCH = SCHED_BATCH,
            IDLE = SCHED_IDLE,            
        };
    };

    class Level
    {
    public :
        using type = uint8_t;
        static constexpr type DEFAULT = 0;
    };

    using Priority = std::tuple<Policies::type, Level::type>;
#endif 

private :
    /**
     * @brief Creates a new thread object.
     * 
     * This method is used to create a new thread object, which can be used to manage a thread's lifetime.
     * 
     * @return A shared pointer to the newly created thread object.
     */
    static auto __create() noexcept -> std::shared_ptr<Thread>;

public :
    /**
     * @brief Destructor for the Thread class.
     * 
     * This destructor is used to release any resources held by the thread object when it is destroyed.
     */
    virtual ~Thread() noexcept = default;

public :
    /**
     * @brief Creates a new thread and executes the given function asynchronously.
     * 
     * This method creates a new thread and executes the given function in a separate thread.
     * 
     * @param func The function to be executed in the new thread.
     * @return A future object to synchronize the execution of the function.
     */
    static auto async(std::function<void()>&& func) noexcept -> std::future<void>;

public :
    /**
     * @brief Starts the thread with the given function.
     * 
     * This method is used to start the thread with the given function. The function will be executed in a separate thread.
     * 
     * @param func The function to be executed in the new thread.
     * @return A future object to synchronize the execution of the function.
     */
    virtual auto start(std::function<void()>&& func) noexcept -> std::future<void> = 0;

    /**
     * @brief Detaches the thread from the thread object.
     * 
     * This method is used to detach the thread from the thread object, allowing it to run independently.
     * 
     * Note that once a thread is detached, it cannot be joined or waited on using the join() function.
     */
    virtual auto detach() noexcept -> void = 0;

    /**
     * @brief Joins the thread with the current thread.
     * 
     * This method is used to join the thread with the current thread, blocking until the thread completes its execution.
     */
    virtual auto join() noexcept -> void = 0;

    /**
     * @brief Sets the priority of the thread.
     * 
     * This method is used to set the priority of the thread, which can affect its scheduling behavior.
     * 
     * @param priority The new priority of the thread.
     * @return True if the priority was successfully set, false otherwise.
     */
    virtual auto set_priority(const Priority& priority) noexcept -> bool = 0;

    /**
     * @brief Gets the current priority of the thread.
     * 
     * This method is used to get the current priority of the thread.
     * 
     * @return The current priority of the thread.
     */
    virtual auto get_priority() noexcept -> Priority = 0;
};
} // namespace common