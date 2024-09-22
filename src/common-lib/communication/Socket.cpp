#include "common-lib/communication/Socket.hpp"
#include "common-lib/exception/Exception.hpp"

#include <string>

#if defined(WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(LINUX)
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#endif

namespace common
{
namespace detail
{
class DetailSocket : public Socket
{
private :
    int32_t _fd = -1; // server's fd
    int32_t _conn = -1; // if server, client's fd. otherwise server's fd
    int32_t _port = -1;
    std::string _address;

protected :
    auto get_fd() const -> int32_t { return _fd; }
    auto set_fd(int32_t fd) -> void { _fd = fd; }
    auto get_conn() const -> int32_t { return _conn; }
    auto set_conn(int32_t conn) -> void { _conn = conn; }
    auto get_port() const -> int32_t { return _port; }
    auto set_port(int32_t port) -> void { _port = port; }
    auto get_address() const -> const std::string& { return _address; }
    auto set_address(const std::string& address) -> void { _address = address;}

public :
    auto prepare(const std::string& address, const int32_t port) -> void override
    {
        set_address(address);
        set_port(port);

    #if defined(WIN32)
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
        {
            throw exception::SocketErrorException();
        }
    #endif

        set_fd(socket(AF_INET, SOCK_STREAM, 0));
        if(get_fd() == 0)
        {
            const int32_t errNo = errno;
            std::string what("Socket error:");
            what += strerror(errNo);
            close();
            throw exception::SocketErrorException();
        }
    }

    auto close() -> void override
    {
    #if defined(WIN32)
        closesocket(get_fd());
        WSACleanup();
    #elif defined(LINUX)
        ::close(get_fd());
    #endif
    }

    auto read(void* buffer, const size_t size) -> size_t override
    {
        size_t readSize = 0;
    #if defined(WIN32)
        if((readSize = recv(get_conn(), reinterpret_cast<char*>(buffer), size, 0)) < 0) 
        {
            const int32_t errNo = errno;
            std::string what("Socket error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketError, what);
        }
    #elif defined(LINUX)
        if((readSize = ::read(get_conn(), buffer, bufferSize)) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Socket error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketError, what);
        }
    #endif
        return readSize;
    }

    auto read(char* buffer, const size_t size) -> size_t override
    {
        return read(reinterpret_cast<void*>(buffer), size);
    }

    auto read(void* buffer, const size_t size, const uint32_t millisecond) -> size_t override
    {
        size_t readSize = 0;
    #if defined(WIN32)

    #elif defined(LINUX)
        struct timeval val;
        val.tv_sec = 0;
        val.tv_usec = millisecond * 1000;

        fd_set readFd;
        FD_ZERO(&readFd);
        FD_SET(get_conn(), &readFd);
        select(get_conn() + 1, &readFd, NULL, NULL, &val);

        if(FD_ISSET(get_conn(), &readFd))
        {
            read(buffer, bufferSize);
        }
    #endif
        return readSize;
    }

    auto read(char* buffer, const size_t size, const uint32_t millisecond) -> size_t override
    {
        return read(reinterpret_cast<void*>(buffer), size, millisecond);
    }

    auto send(void* buffer, const size_t size) -> void override
    {
    #if defined(WIN32)
        if(::send(get_conn(), reinterpret_cast<char*>(buffer), size, 0) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Socket error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketError, what);
        }
    #elif defined(LINUX)
        if(::send(get_conn(), buffer, size, 0) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Socket error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketError, what);
        }
    #endif
    }    

    auto send(const char* buffer, const size_t size) -> void override
    {
        send(reinterpret_cast<void*>(const_cast<char*>(buffer)), size);
    }
};

class Server final : public DetailSocket
{
public :
    ~Server() final = default;

public :
    auto open() -> void override
    {
        sockaddr_in addrIn;
        addrIn.sin_family = AF_INET;
        addrIn.sin_addr.s_addr = INADDR_ANY;
        addrIn.sin_port = htons(get_port());

    #if defined(LINUX)
        int32_t opt = 1;
        if(::setsockopt(get_fd(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) 
        {
            const int32_t errNo = errno;
            std::string what("Socket setsockopt error: ");
            what += strerror(errNo);
            close();
            throw exception::Exception(exception::SocketError, what); 
        }
    #endif

        if(::bind(get_fd(), reinterpret_cast<sockaddr*>(&addrIn), sizeof(addrIn)) < 0)
        {
            const int32_t errNo = errno;
            std::string what("Server bind error: ");
            what += strerror(errNo);
            close();
            throw exception::Exception(exception::SocketServerError, what);
        }

    #if defined(WIN32)
        if(::listen(get_fd(), SOMAXCONN) < 0)
    #elif defined(LINUX)
        if(::listen(get_fd(), BACKLOG) < 0)
    #endif
        {
            const int32_t errNo = errno;
            std::string what("Server listen error: ");
            what += strerror(errNo);
            close();
            throw exception::Exception(exception::SocketServerError, what); 
        }

        int32_t addrLen = sizeof(addrIn);
        set_conn(accept(get_fd(), reinterpret_cast<sockaddr*>(&addrIn), &addrLen));
        if(get_conn() < 0)
        {
            const int32_t errNo = errno;
            std::string what("Server accept error: ");
            what += strerror(errNo);
            close();
            throw exception::Exception(exception::SocketServerError, what);
        }
    }
};

class Client final : public DetailSocket
{
public :
    ~Client() = default;

public :
    auto open() -> void override
    {
    #if defined(WIN32)
        sockaddr_in addrIn;
        addrIn.sin_family = AF_INET;
        addrIn.sin_addr.s_addr = inet_addr(get_address().c_str());
        addrIn.sin_port = htons(get_port());
    #elif defined(LINUX)
        sockaddr_in addrIn;
        addrIn.sin_family = AF_INET;
        addrIn.sin_port = htons(get_port());

        if(inet_pton(AF_INET, address().c_str(), &addrIn.sin_addr) <= 0)
        {
            const int32_t errNo = errno;
            std::string what("Client inet_pton error:");
            what += strerror(errNo);
            throw exception::Exception(exception::SocketClientError, what);
        }
    #endif

        if (connect(get_fd(), reinterpret_cast<sockaddr*>(&addrIn), sizeof(addrIn)) < 0) 
        {
            const int32_t errNo = errno;
            std::string what("Client connect error:");
            what += strerror(errNo);
            close();
            throw exception::Exception(exception::SocketClientError, what);
        }
        else
        {
            set_conn(get_fd());
        }
    }
};
} // namespace detail

auto Socket::__create(SocketType::type socketType) -> std::shared_ptr<Socket>
{
    return socketType == SocketType::SERVER ? std::shared_ptr<Socket>(new detail::Server(), 
                                              [](detail::Server* obj){
        obj->close();
    }) : std::shared_ptr<Socket>(new detail::Client(), 
                                 [](detail::Client* obj){
        obj->close();
    });
};
} // namespace common::communication

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
} // namespace common

#endif