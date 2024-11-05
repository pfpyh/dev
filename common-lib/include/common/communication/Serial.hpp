#pragma once

#include <string>

#include "common/NonCopyable.hpp"
#include "common/Factory.hpp"

#if defined(WINDOWS)
#include <windows.h>
#elif defined(LINUX)
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

namespace common
{
static constexpr uint8_t SERIAL_READ = 0x01;
static constexpr uint8_t SERIAL_WRITE = 0x02;

class Baudrate
{
public :
    enum type : uint8_t
    {
        _9600,
        _14400,
        _19200,
        _38400,
        _57600,
        _115200,
        _128000,
        _256000,
    };
};

class Serial : public NonCopyable
             , public Factory<Serial>
{
    friend class Factory<Serial>;

public :
    virtual ~Serial() = default;

public :
    virtual auto open(const std::string& port,
                      const Baudrate::type baudrate,
                      const uint8_t mode) noexcept -> bool = 0;
    virtual auto close() noexcept -> void = 0;
    virtual auto is_open() noexcept -> bool = 0;
    virtual auto readline() noexcept -> std::string = 0;
    virtual auto write(const char* buffer, const size_t size) noexcept -> bool = 0;

public :
    static auto __create() noexcept -> std::shared_ptr<Serial>;
};

namespace detail
{
class SerialHandler
{
public :
    virtual ~SerialHandler() = default;

#if defined(WINDOWS)
public :
    virtual auto Wrapper_CreateFile(LPCSTR lpFileName,
                                    DWORD dwDesiredAccess,
                                    DWORD dwShareMode,
                                    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                    DWORD dwCreationDisposition,
                                    DWORD dwFlagsAndAttributes,
                                    HANDLE hTemplateFile) -> HANDLE;

    virtual auto Wrapper_CloseHandle(HANDLE hObject) -> void;

    virtual auto Wrapper_ReadFile(HANDLE hFile,
                                  LPVOID lpBuffer,
                                  DWORD nNumberOfBytesToRead,
                                  LPDWORD lpNumberOfBytesRead,
                                  LPOVERLAPPED lpOverlapped) -> bool;

    virtual auto Wrapper_WriteFile(HANDLE hFile,
                                   LPCVOID lpBuffer,
                                   DWORD nNumberOfBytesToWrite,
                                   LPDWORD lpNumberOfBytesWritten,
                                   LPOVERLAPPED lpOverlapped) -> bool;  

    virtual auto Wrapper_GetCommState(HANDLE hFile,
                                      LPDCB lpDCB) -> bool;

    virtual auto Wrapper_SetCommState(HANDLE hFile,
                                      LPDCB lpDCB) -> bool;

    virtual auto Wrapper_SetCommTimeouts(HANDLE hFile,
                                         LPCOMMTIMEOUTS lpCommTimeouts) -> bool;
#elif defined(LINUX)
    virtual auto Wrapper_Open(const std::string& port, int32_t mode) -> int32_t;
    virtual auto Wrapper_Close(int32_t fd) -> void;
    virtual auto Wrapper_Read(int32_t fd, char* buffer, size_t size) -> ssize_t;
    virtual auto Wrapper_Write(int32_t fd, const char* buffer, size_t size) -> bool;
#endif
};

class DetailSerial : public Serial
{
private :
    std::shared_ptr<SerialHandler> _handler;
    bool _isOpen = false;

#if defined(WINDOWS)
    HANDLE _handle;
#elif defined(LINUX)
    int32_t _fd;
#endif

public :
    DetailSerial()
        : _handler(std::make_shared<SerialHandler>()) {}
    DetailSerial(std::shared_ptr<SerialHandler> handler)
        : _handler(handler) {}

public :
    auto open(const std::string& port,
              const Baudrate::type baudRate,
              const uint8_t mode) noexcept -> bool override;
    auto close() noexcept -> void override;
    inline auto is_open() noexcept -> bool override { return _isOpen; }
    auto readline() noexcept -> std::string override;
    auto write(const char* buffer, const size_t size) noexcept -> bool override;
};
} // namespace detail
} // namespace common
