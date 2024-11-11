#pragma once

#include "math/matrix.hpp"
#include "math/angle.hpp"

#include <stdint.h>

namespace common::math
{
template <typename T>
class KalmanFilter
{
private :
    T _H;
    T _Q;
    T _R;
    T _x;
    T _P;
    bool _first = true;

public :
    KalmanFilter(const T& H,
                 const T& Q,
                 const T& R,
                 const T& x,
                 const T& P)
        : _H(H), _Q(Q), _R(R), _x(x), _P(P) {};

    auto run(const T& A, const T& z) -> const decltype(_x)&
    {
        if (!_first)
        {
            // predict
            auto xp = A * _x;
            auto Pp = A * _P * A.transpose() + _Q;

            // kalman gain
            auto K = (Pp * _H.transpose()) * util::inverse(_H * Pp * _H.transpose() + _R);

            // estimate
            _x = xp + K * (z - (_H * xp));
            _P = Pp - K * _H * Pp;
        }
        else _first = false;
        return _x;
    };
};
} // namespace common::math