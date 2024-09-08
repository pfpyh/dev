#if defined(LINUX)

#include "common-lib/communication/Socket.hpp"
#include "common-lib/exception/Exception.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>

namespace common::communication
{
namespace detail
{
class Server : public Socket
{
private :
    static constexpr int32_t BACKLOG = 5;

public :
    ~Server() final = default;

public :
    auto open() -> void override
    {
        int32_t opt = 1;
        if(::setsockopt(fd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
        {
            const int32_t errNo = errno;
            std::string what("Socket setsockopt error: ");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketError, what); 
        }

        sockaddr_in addrIn;
        addrIn.sin_family = AF_INET;
        addrIn.sin_port = htons(port());
        addrIn.sin_addr.s_addr = INADDR_ANY;

        if(::bind(fd(), reinterpret_cast<sockaddr*>(&addrIn), sizeof(addrIn)) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Server bind error: ");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketServerError, what);
        }

        if(::listen(fd(), BACKLOG) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Server listen error: ");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketServerError, what); 
        }

        int32_t addrLen = sizeof(addrIn);
        int32_t clientFd = -1;
        if((clientFd = accept(fd(), reinterpret_cast<sockaddr*>(&addrIn), reinterpret_cast<socklen_t*>(&addrLen))) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Server accept error: ");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketServerError, what);
        }

        connFd(clientFd);
    }
};

class Client : public Socket
{
private :
    int32_t _fd = -1;

public :
    ~Client() final = default;

public :
    auto open() -> void override
    {
        sockaddr_in addrIn;
        addrIn.sin_family = AF_INET;
        addrIn.sin_port = htons(port());

        if(inet_pton(AF_INET, address().c_str(), &addrIn.sin_addr) <= 0)
        {
            const int32_t errNo = errno;
            std::string what("Client inet_pton error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketClientError, what);
        }

        if(::connect(fd(), reinterpret_cast<sockaddr*>(&addrIn), sizeof(addrIn)) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Client connect error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketClientError, what);
        }

        connFd(fd());
    }
};
} // namespace detail

auto Socket::create_server() noexcept -> std::shared_ptr<Socket>
{
    return std::shared_ptr<detail::Server>(new detail::Server(), [](detail::Server* obj)
    {
        obj->close();
        delete obj;
        obj = nullptr;
    });
}

auto Socket::create_client() noexcept -> std::shared_ptr<Socket>
{
    return std::shared_ptr<detail::Client>(new detail::Client(), [](detail::Client* obj)
    {
        obj->close();
        delete obj;
        obj = nullptr;
    });
}

auto Socket::prepare(const std::string& address, const int32_t port) -> void
{
    _address = address;
    _port = port;

    if((_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        const int32_t errNo = errno;
        std::string what("Socket error:");
        what += strerror(errNo);
        throw exception::Exception(exception::SocketError, what); 
    }
}

auto Socket::close() noexcept -> void
{
    ::close(_fd);
}

auto Socket::read(void* buffer, const size_t bufferSize) -> void
{
    if(::read(connFd(), buffer, bufferSize) < 0)
    {
        const int32_t errNo = errno;
        std::string what("Socket error:");
        what += strerror(errNo);
        throw exception::Exception(exception::SocketError, what);
    }
}

auto Socket::read(void* buffer, const size_t bufferSize, const uint32_t millisecond) -> void
{
    struct timeval val;
    val.tv_sec = 0;
    val.tv_usec = millisecond * 1000;

    fd_set readFd;
    FD_ZERO(&readFd);
    FD_SET(connFd(), &readFd);
    select(connFd() + 1, &readFd, NULL, NULL, &val);

    if(FD_ISSET(connFd(), &readFd))
    {
        read(buffer, bufferSize);
    }
}

auto Socket::send(void* buffer, const size_t bufferSize) -> void
{
    if(::send(connFd(), buffer, bufferSize, 0) < 0)
    {
        const int32_t errNo = errno;
        std::string what("Socket error:");
        what += strerror(errNo);
        throw exception::Exception(exception::SocketError, what);
    }
}
} // namespace common::communication

#endif