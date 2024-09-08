#pragma once

#include "common-lib/communication/Socket.hpp"
#include "common-lib/thread/Worker.hpp"
#include "common-lib/exception/Exception.hpp"
#include "common-lib/logging/Logger.hpp"

#include <string>

namespace common::communication
{
template <typename DataType>
class Provider: public thread::SingleWorker<Provider<DataType>>
{
private :
    std::shared_ptr<Socket> _socket = std::move(Socket::create_server());
    std::shared_ptr<std::future<void>> _future;

    static constexpr size_t _bufferSize = sizeof(DataType);
    char* _buffer[_bufferSize] = {0, };
    bool _isOpen = false;

public :
    auto connect(const std::string& address,    
                 const int32_t port) noexcept -> bool
    {
        try
        {
            _socket->prepare(address, port);
            _future = thread::SingleWorker<Provider<DataType>>::run();
        }
        catch(const exception::Exception& e)
        {
            _ERROR_("[common-lib] Provider::connect(%s, %d) : %s", 
                    _socket->address().c_str(), _socket->port(), e.what());
            return false;
        }
        catch(...)
        {
            _ERROR_("[common-lib] Provider::connect(%s, %d) : unknown error",
                    _socket->address().c_str(), _socket->port());
            return false;
        }
        return true;
    }

    auto disconnect() noexcept -> void
    {
        thread::SingleWorker<Provider<DataType>>::stop();
        _future->wait();
        _socket->close();
    }

public :
    auto send(const DataType& data) noexcept -> bool
    {
        try
        {
            void* buffer = static_cast<void*>(&const_cast<DataType&>(data));
            constexpr size_t bufferSize = sizeof(DataType);
            _socket->send(buffer, bufferSize);
            _DEBUG_("[common-lib] Provider::send(%s, %d) : called",
                    _socket->address().c_str(), _socket->port());
        }
        catch(const exception::Exception& e)
        {
            _ERROR_("[common-lib] Provider::send(%s, %d) : %s", 
                    _socket->address().c_str(), _socket->port(), e.what());
        }
        return false;
    }

protected :
    auto __work() -> void
    {
        if(_isOpen == false)
        {
            try
            {
                _socket->open();
                _isOpen = true;
                _DEBUG_("[common-lib] Provider::open(%s, %d) : success", 
                        _socket->address().c_str(), _socket->port());
            }
            catch(const exception::Exception& e)
            {
                thread::SingleWorker<Provider<DataType>>::stop();
                _ERROR_("[common-lib] Provider::open(%s, %d) : %s", 
                    _socket->address().c_str(), _socket->port(), e.what());                
            }
        }        

        try
        {
            _socket->read(static_cast<void*>(_buffer), _bufferSize, 10);
            DataType data;
            memcpy(&data, _buffer, _bufferSize);
        }
        catch(const exception::Exception& e)
        {
            _ERROR_("[common-lib] Provider::read(%s, %d) : %s", 
                    _socket->address().c_str(), _socket->port(), e.what());
        }
    }
};

template <typename DataType>
class Consumer : public thread::SingleWorker<Consumer<DataType>>
{
private :
    std::shared_ptr<Socket> _socket = std::move(Socket::create_client());
    static constexpr size_t _bufferSize = sizeof(DataType);
    char* _buffer[_bufferSize] = {0, };
    std::function<void(DataType&& data)> _onEvent;
    std::shared_ptr<std::future<void>> _future;

public :
    auto connect(const std::string& address,
                 const int32_t port,
                 std::function<void(DataType&& data)>&& onEvent) noexcept -> bool
    {
        try
        {
            _socket->prepare(address, port);
            _socket->open();
            _onEvent = std::move(onEvent);
            _future = thread::SingleWorker<Consumer<DataType>>::run();
            _DEBUG_("[common-lib] Consumer::connect(%s, %d) : success", 
                    _socket->address().c_str(), _socket->port());
        }
        catch(const exception::Exception& e)
        {
            _ERROR_("[common-lib] Consumer::connect(%s, %d) : %s", 
                    _socket->address().c_str(), _socket->port(), e.what());
            return false;
        }
        catch(...)
        {
            _ERROR_("[common-lib] Consumer::connect(%s, %d) : unknown error",
                    _socket->address().c_str(), _socket->port());
            return false;
        }
        return true;
    }

    auto disconeect() noexcept -> void
    {
        thread::SingleWorker<Consumer<DataType>>::stop();
        _future->wait();
        _socket->close();
    }

protected :
    auto __work() -> void
    {
        try
        {
            _socket->read(static_cast<void*>(_buffer), _bufferSize, 10);
            DataType data;
            memcpy(&data, _buffer, _bufferSize);
            _onEvent(std::move(data));
        }
        catch(const exception::Exception& e)
        {
            _ERROR_("[common-lib] Consumer::read(%s, %d) : %s", 
                    _socket->address().c_str(), _socket->port(), e.what());
        }        
    }
};
} // namespace common::communication