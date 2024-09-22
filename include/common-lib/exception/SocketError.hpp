#pragma once

#include "common-lib/exception/BaseException.hpp"

namespace common::exception
{
class SocketErrorException : public Exception
{
public :
    SocketErrorException() noexcept
        : Exception(ExceptionType::SocketError) {}
};
} // namespace common::exception