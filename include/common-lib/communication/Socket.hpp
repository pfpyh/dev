#pragma once

#include "common-lib/NonCopyable.hpp"

#include <stdint.h>
#include <string>
#include <memory>

namespace common::communication
{
class Socket : public NonCopyable
{
private :
    int32_t _fd = -1;
    int32_t _connFd = -1;

    int32_t _port = -1;
    std::string _address;

public :
    inline auto fd() const noexcept -> int32_t { return _fd; }
    inline auto connFd() const noexcept -> int32_t { return _connFd; }
    inline auto port() const noexcept -> int32_t { return _port; }
    inline auto address() const noexcept -> const std::string& { return _address; }

protected :
    inline auto connFd(const int32_t fd) noexcept -> void { _connFd = fd; }

public :
    virtual ~Socket() = default;

public :
    /**
     * @brief Server socket을 생성한다.
     * 
     * @return Socket 객체
     */
    static auto create_server() noexcept -> std::shared_ptr<Socket>;

    /**
     * @brief Client socket을 생성한다.
     * 
     * @return Socket 객체
     */
    static auto create_client() noexcept -> std::shared_ptr<Socket>;

public :
    /**
     * @brief Socket을 생성한다.
     * 
     * @param address Server의 주소. 공백으로 제공되면 현재 주소를 사용한다.
     * @param port Server의 port 번호
     * 
     * @throw SocketError
     */
    auto prepare(const std::string& address, const int32_t port) -> void;

    /**
     * @brief Server의 경우 listen이 수행된다. Client의 경우 server에 연결을 시도한다.
     * 
     * @throw SocketServerError / SocketClientError
     */
    virtual auto open() -> void = 0;

    /**
     * @brief Socket의 연결을 닫는다. create를 통해 생성된 경우, 객체 소멸 시 자동으로 호출된다.
     */
    auto close() noexcept -> void;

    /**
     * @brief Socket을 통해서 message를 수신한다. (동기)
     * 
     * @param buffer Message가 담겨있는 buffer.
     * @param size Buffer의 size.
     * 
     * @throw SocketError
     */
    auto read(void* buffer, const size_t size) -> void;

    /**
     * @brief Socket을 통해서 message를 수신한다. (동기)
     * 
     * @param buffer Message가 담겨있는 buffer.
     * @param size Buffer의 size.
     * @param millisecond Timeout 시간. (ms)
     * 
     * @throw SocketError
     */
    auto read(void* buffer, const size_t size, const uint32_t millisecond) -> void;

    /**
     * @brief Socket을 통해 message를 전송한다. (동기)
     * 
     * @param buffer Message가 담겨있는 buffer.
     * @param size Buffer의 size.
     * 
     * @throw SocketError
     */
    auto send(void* buffer, const size_t size) -> void;
};
} // namespace common::communication