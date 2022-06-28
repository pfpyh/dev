#pragma once

namespace pattern
{

template <typename ClassName>
class Singleton
{
protected:
    Singleton() = default;

public:
    Singleton( const Singleton & ) = delete;
    Singleton & operator=( const Singleton & ) = delete;
    Singleton( Singleton && ) = delete;
    Singleton & operator=( Singleton && ) = delete;

    static auto GetInstance() -> ClassName &
    {
        static ClassName instance;
        return instance;
    }
};

} // namespace pattern