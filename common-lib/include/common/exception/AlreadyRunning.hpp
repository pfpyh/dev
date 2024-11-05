#pragma once

#include "common/exception/BaseException.hpp"

namespace common::exception
{
class AlreadyRunningException : public Exception
{
public :
    AlreadyRunningException() noexcept
        : Exception(ExceptionType::AlreadyRunning) {}
};
} // namespace common::exception