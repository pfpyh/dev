#pragma once

#include "common/exception/BaseException.hpp"

namespace common::exception
{
class SocketErrorException : public Exception
{
public :
    SocketErrorException() noexcept
        : Exception(ExceptionType::SocketError) {}
};
} // namespace common::exception