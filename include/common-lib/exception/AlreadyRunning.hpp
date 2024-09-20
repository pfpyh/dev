#pragma once

#include "common-lib/exception/BaseException.hpp"

namespace common::exception
{
class AlreadyRunningException : public Exception
{
public :
    AlreadyRunningException() noexcept
        : Exception(ExceptionType::AlreadyRunning) {}
};
} // namespace common::exception