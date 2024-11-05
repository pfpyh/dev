#include "common/exception/Exception.hpp"

#include <unordered_map>

namespace common::exception
{
namespace detail
{
std::unordered_map<ExceptionType, std::string> ExceptionStringMap = {
    {Undefined, "Undefined exception."},
    {AlreadyRunning, "Ojbect already started."},
    {SocketError, "Socket open failure."},
    {SocketServerError, "Socket server failure."},
    {SocketClientError, "Socket client failure."},
    {LimitExceed, "Exceed valid range."},
};
} // namespace detail

Exception::Exception(const ExceptionType type) noexcept
    : _type(type) {}

Exception::Exception(const ExceptionType type, const std::string& what) noexcept
    : _type(type)
    , _what(what) {}

const char* Exception::what() const noexcept
{
    if(false == _what.empty()) { return _what.c_str(); }
    auto itor = detail::ExceptionStringMap.find(_type);
    if(itor != detail::ExceptionStringMap.end()) { return detail::ExceptionStringMap.find(_type)->second.c_str(); }
    else { return detail::ExceptionStringMap.find(Undefined)->second.c_str(); }        
}
} // namespace common::exception