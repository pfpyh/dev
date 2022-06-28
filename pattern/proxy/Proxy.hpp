#pragma once

#include "ProxyBase.hpp"

namespace pattern
{

class Proxy
{
protected :
    ProxyBase * base = nullptr;

public :
    virtual ~Proxy() {};

    auto registProxy( ProxyBase * base ) -> void { this->base = base; }
    auto unregistProxy() -> void { this->base = nullptr; }
};

} // namespace pattern