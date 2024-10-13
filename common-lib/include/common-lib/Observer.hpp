#pragma once

#include <vector>

namespace common
{
template <typename... Args>
class Observer
{
public :
    virtual auto onEvent(Args... args) -> void = 0;
};

template <typename... Args>
class Subject
{
private :
    std::vector<Observer<Args...>*> _observers;

public :
    auto regist(Observer<Args...>* observer) -> void
    {
        _observers.emplace_back(observer);
    }

    auto unregist(Observer<Args...>* observer) -> void
    {
        _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
    }

    auto notify(Args... args) -> void
    {
        for(auto observer : _observers)
        {
            observer->onEvent(args...);
        }
    }
};
};