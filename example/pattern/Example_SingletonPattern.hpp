#pragma once

#include "singleton/Singleton.hpp"

#include "lockable/SpinLock.hpp"
#include "lockable/ScopedLock.hpp"

#include "thread/ThreadBase.hpp"

#include <string>
#include <stdint.h>
#include <unordered_map>
#include <iostream>

namespace pattern::example::singleton
{

class ResultManager : public Singleton<ResultManager>
{
public :
    using Score = int32_t;
    using Subject = std::string;
    using Result = std::pair<Subject, Score>;

private :
    lockable::SpinLock lock;
    std::unordered_map<Subject, Score> map;

public :
    auto Add( const Result & result ) -> void
    {
        lockable::ScopedLock<decltype(lock)> scopedLock( lock );
        map.insert( result );
    };

    auto Add( const Subject & subject, const Score score ) -> void
    {
        lockable::ScopedLock<decltype(lock)> scopedLock( lock );
        map.insert( Result( subject, score ) );
    }

    auto Find( const Subject & sub ) -> Score
    {
        lockable::ScopedLock<decltype(lock)> scopedLock( lock );
        const Score rtn = map.end() != map.find( sub ) ? map[sub] : -1;
        return rtn;
    };
};

auto Example() -> void
{
    std::vector<std::future<bool>> futures;
    
    thread::AsyncThread<bool> t1( []()->bool {
        ResultManager::GetInstance().Add( "Temp1", 1 );
        return true;
    } );

    thread::AsyncThread<bool> t2( []()->bool {
        ResultManager::GetInstance().Add( "Temp2", 2 );
        return true;
    } );

    thread::AsyncThread<bool> t3( []()->bool {
        ResultManager::GetInstance().Add( "Temp3", 3 );
        return true;
    } );

    futures.push_back( std::move( t3.Run() ) );
    futures.push_back( std::move( t2.Run() ) );
    futures.push_back( std::move( t1.Run() ) );

    for( const auto & future : futures ) future.wait();

    std::cout << "Temp1 = " << ResultManager::GetInstance().Find( "Temp1" ) << std::endl;
    std::cout << "Temp2 = " << ResultManager::GetInstance().Find( "Temp2" ) << std::endl;
    std::cout << "Temp3 = " << ResultManager::GetInstance().Find( "Temp3" ) << std::endl;
}

} // namespace pattern::example::singleton