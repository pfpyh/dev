#pragma once

#include "CommonHeader.hpp"
#include "common/exception/BaseException.hpp"

namespace common::exception
{
class COMMON_LIB_API SocketErrorException : public Exception
{
public :
    SocketErrorException() noexcept
        : Exception(ExceptionType::SocketError) {}
};
} // namespace common::exception