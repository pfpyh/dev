#pragma once

#include <exception>
#include <string>

namespace common::exception
{
enum ExceptionType : uint8_t
{
    Undefined = 0,
    AlreadyRunning,
    SocketError,
    SocketServerError,
    SocketClientError,
    LimitExceed,
};

class Exception : public std::exception
{
private :
    ExceptionType _type = Undefined;
    std::string _what;

public :
    Exception(const ExceptionType type) noexcept;
    Exception(const ExceptionType type, const std::string& what) noexcept;
    const char* what() const noexcept override;
};
} // namespace common::exception