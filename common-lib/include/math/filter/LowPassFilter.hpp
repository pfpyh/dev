#pragma once

namespace common::math
{
template <typename T>
class LowPassFilter
{
private:
    T _prev;
    T _alpha ;
    bool _first = true;

public:
    LowPassFilter(const T& init, const T& alpha)
        : _prev(init), _alpha(alpha) {};

    auto run(const T& x)->T
    {
        if (_first)
        {
            _prev = x;
            _first = false;
        }
        _prev = _alpha * _prev + (1 - _alpha) * x;
        return _prev;
    };
};
} // namespace common::math