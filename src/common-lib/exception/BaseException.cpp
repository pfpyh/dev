/*
MIT License

Copyright (c) 2024, Younghwan Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "common-lib/exception/Exception.hpp"

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