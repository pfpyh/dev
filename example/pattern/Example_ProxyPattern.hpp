#pragma once

#include "proxy/Proxy.hpp"
#include "proxy/ProxyBase.hpp"

#include <string>

namespace pattern::example::proxy
{

///////////////////////////////////
// Implemented by Interface side //
///////////////////////////////////
class SensorConnector
{
public :
    auto Connect( const std::string & address ) -> bool
    {
        // Do connect to somewhere
    };

    auto Disconnect() -> bool
    {
        // Do disconnect from somewhere
    };
};

class DriverConnector
{
public :
    auto GetDriverIns() -> const char *
    {
        // Do driver prepare
        return "Address";
    };

    auto Connect( const char * ins ) -> void
    {
        // Do connect driver
    };

    auto Disconnect( const char * ins ) -> void
    {
        // Do disconnect driver
    };
};

////////////////////////////////////
// Implemented by Middleware side //
////////////////////////////////////

class SensorConnProxyBase : public ProxyBase
{
private :
    SensorConnector conn;
    static constexpr char ADDRESS[] = "address";

public :
    virtual auto Connect() -> bool override
    {
        return conn.Connect( ADDRESS );
    };

    virtual auto Disconnect() -> bool override
    {
        return conn.Disconnect();
    };
};

class DriverConnProxyBase : public ProxyBase
{
private :
    DriverConnector conn;

public :
    virtual auto Connect() -> bool override
    {
        const char * ins = conn.GetDriverIns();
        if( !ins ) conn.Connect( ins );
        return ins;
    };

    virtual auto Disconnect() -> bool override
    {
        const char * ins = conn.GetDriverIns();
        if( !ins ) conn.Disconnect( ins );
        return ins;
    };
};

class ConnectorProxy : public Proxy
{
public :
    auto Connect() -> bool { return base->Connect(); };
    auto Disconnect() -> bool { return base->Disconnect(); };
};

////////////////////////////////////
////////////////////////////////////
////////////////////////////////////

ConnectorProxy g_proxy;
bool g_sensor_base = true;

auto Example_interface() -> void
{
    ProxyBase * base = nullptr; // This should be delete somewhere !
    if( g_sensor_base ) base = new SensorConnProxyBase();
    else base = new DriverConnProxyBase();
    g_proxy.registProxy( base );
}

auto Example_Core() -> void
{
    g_proxy.Connect();
    // Do something
    g_proxy.Disconnect();
}

} // namespace pattern::example::proxy