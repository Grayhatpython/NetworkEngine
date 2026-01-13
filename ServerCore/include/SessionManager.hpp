#pragma once 

namespace servercore
{
    class Session;
    class SessionManager
    {
    public:
        explicit SessionManager();

    public:
        void                        AddSession(std::shared_ptr<Session> session);
	    void                        RemoveSession(std::shared_ptr<Session> session);
        std::shared_ptr<Session>    CreateSession();

	    void                                            SetSessionFactory(std::function<std::shared_ptr<Session>()> sessionFactory) { _sessionFactory = sessionFactory; }
        std::function<std::shared_ptr<Session>(void)>   GetSessionFactory() { return _sessionFactory; }
        uint64                                          GetSessionCount();

    private:
        Lock                                            _lock;
        std::unordered_set<std::shared_ptr<Session>>    _sessions;
        std::function<std::shared_ptr<Session>(void)>   _sessionFactory;
        uint64                                          _sessionCount = 0;
    };
}