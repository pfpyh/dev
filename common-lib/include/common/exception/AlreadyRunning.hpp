#pragma once

#include "CommonHeader.hpp"
#include "common/exception/BaseException.hpp"

namespace common::exception
{
class COMMON_LIB_API AlreadyRunningException : public Exception
{
public :
    AlreadyRunningException() noexcept
        : Exception(ExceptionType::AlreadyRunning) {}
};
} // namespace common::exception