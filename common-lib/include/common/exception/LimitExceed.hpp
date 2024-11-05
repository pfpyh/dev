#pragma once

#include "common/exception/BaseException.hpp"

namespace common::exception
{
class LimitExceedException : public Exception
{
public :
    LimitExceedException() noexcept
        : Exception(ExceptionType::LimitExceed) {}
};
} // namespace common::exception