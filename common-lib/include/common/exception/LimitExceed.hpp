#pragma once

#include "CommonHeader.hpp"
#include "common/exception/BaseException.hpp"

namespace common::exception
{
class COMMON_LIB_API LimitExceedException : public Exception
{
public :
    LimitExceedException() noexcept
        : Exception(ExceptionType::LimitExceed) {}
};
} // namespace common::exception