#pragma once

#include <stdint.h>
#include <mutex>
#include <queue>
#include <memory>
#include <future>

namespace pattern::thread
{

template<typename ReturnType, uint8_t MAX_THREAD_COUNT>
class ThreadPool
{
protected :
    std::queue<std::shared_ptr<std::promise<void>>> promises;
    std::mutex lock;

public :
    auto AddWork( std::function<ReturnType()> work ) -> std::shared_ptr<std::promise<ReturnType>>
    {
        auto promise = std::make_shared<std::promise<ReturnType>>();
        std::thread thread( [this, promise, work]()
        {
            auto work_promise = std::make_shared<std::promise<void>>();
            auto work_future = std::make_shared<std::future<void>>( work_promise->get_future() );
            work_mutex.lock();
            if( work_count == max_work_count )
            {
                promises.emplace( std::move( work_promise ) );
                work_mutex.unlock();
                work_future->get();
                work_mutex.lock();
                ++work_count;
            }
            else ++work_count;
            work_mutex.unlock();

            promise->set_value( work() );

            std::lock_guard<std::mutex> lock_guard( work_mutex );
            if( !promises.empty() )
            {
                promises.front()->set_value();
                promises.pop();
            }
            --work_count;
        } );
        thread.detach();
        return promise;
    }
};

} // namespace pattern::thread