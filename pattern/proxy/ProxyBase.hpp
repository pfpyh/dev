#pragma once

namespace pattern
{

class ProxyBase
{
public :
    virtual ~ProxyBase() {};

public :
    virtual auto Connect() -> bool = 0;
    virtual auto Disconnect() -> bool = 0;
};

} // namespace pattern

