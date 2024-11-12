#pragma once

#include <vector>
#include <memory>
#include <algorithm>

namespace common
{
template <typename... Args>
class BaseObserver
{
public :
    virtual ~BaseObserver() = default;
    virtual auto onEvent(Args... args) -> void = 0;
};

template <typename Derived, typename... Args>
class Observer : public BaseObserver<Args...> 
               , public std::enable_shared_from_this<Observer<Derived, Args...>>
{
public :
    template <typename... ConstructTypes>
    static auto create(ConstructTypes... types) -> std::shared_ptr<Derived>
    {
        return std::shared_ptr<Derived>(new Derived(std::forward<ConstructTypes>(types)...));
    }

    auto get_ptr() noexcept -> std::shared_ptr<Observer<Derived, Args...>>
    {
        return this->shared_from_this();
    }

public :
    virtual auto onEvent(Args... args) -> void = 0;
};

template <typename... Args>
class Subject
{
private :
    std::vector<std::shared_ptr<BaseObserver<Args...>>> _observers;

public :
    auto regist(BaseObserver<Args...>& observer) noexcept -> void
    {
        _observers.emplace_back(observer.get_ptr());
    }

    auto regist(std::shared_ptr<BaseObserver<Args...>> observer) noexcept -> void
    {
        _observers.emplace_back(std::move(observer));
    }

    auto unregist(BaseObserver<Args...>& observer) noexcept -> void
    {
        // auto target = observer.get_ptr();
        // _observers.erase(std::remove(_observers.begin(), _observers.end(), target), _observers.end());

                auto target = observer.get_ptr();
        _observers.erase(
            std::remove_if(_observers.begin(), _observers.end(),
                           [&target](const std::shared_ptr<BaseObserver<Args...>>& obs) {
                               return obs == target;
                           }),
            _observers.end());
    }

    auto unregist(std::shared_ptr<BaseObserver<Args...>> observer) noexcept -> void
    {
        // _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());

        _observers.erase(
            std::remove_if(_observers.begin(), _observers.end(),
                           [&observer](const std::shared_ptr<BaseObserver<Args...>>& obs) {
                               return obs == observer;
                           }),
            _observers.end());
    }

    auto notify(Args... args) -> void
    {
        for(auto observer : _observers) { observer->onEvent(args...); }
    }
};
};