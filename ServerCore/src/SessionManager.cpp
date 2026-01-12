#include "Pch.hpp"
#include "SessionManager.hpp"
#include "Session.hpp"

namespace servercore
{
    SessionManager::SessionManager(std::function<std::shared_ptr<Session>()> sessionFactory)
        :   _sessionFactory(sessionFactory)
    {

    }

    void SessionManager::AddSession(std::shared_ptr<Session> session)
    {
        if(session == nullptr)
            return;

        WriteLockGuard lockGuard(_lock);
        _sessions.insert(session);
    }

    void SessionManager::RemoveSession(std::shared_ptr<Session> session)
    {
        if(session == nullptr)
            return;

        WriteLockGuard lockGuard(_lock);
        _sessions.erase(session);
    }

    std::shared_ptr<Session> SessionManager::CreateSession()
    {
        auto session = _sessionFactory();
        if(session == nullptr)
            return nullptr;

        //  TODO
        return session;
    }

    uint64 SessionManager::GetSessionCount()
    {
        uint64 count = 0;
        {
            WriteLockGuard lockGuard(_lock);
            count = _sessions.size();
        }
        
        return count;
    }
}