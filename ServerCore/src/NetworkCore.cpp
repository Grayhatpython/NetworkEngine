#include "Pch.hpp"
#include "NetworkCore.hpp"
#include "Session.hpp"
#include "Acceptor.hpp"
#include "NetworkUtils.hpp"
#include "SessionManager.hpp"
#include "NetworkDispatcher.hpp"

namespace servercore
{
    INetworkCore::INetworkCore(std::function<std::shared_ptr<Session>()> sessionFactory)
    {
        Initialize(sessionFactory);
    }

    INetworkCore::~INetworkCore()
    {

    }

    void INetworkCore::Stop()
    {
        GSessionManager->Clear();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        auto epollDispatcher = std::static_pointer_cast<EpollDispatcher>(_networkDispatcher);
        epollDispatcher->PostCoreShutdown();

        GlobalContext::GetInstance().Clear();
    }

    DispatchResult INetworkCore::NetworkDispatch(uint32 timeoutMs)
    {
        if(_networkDispatcher)
        {
            auto dispatchReuslt =  _networkDispatcher->Dispatch(timeoutMs);
            return dispatchReuslt;
        }

        return DispatchResult::InvalidDispatcher;
    }

    void INetworkCore::Initialize(std::function<std::shared_ptr<Session>()> sessionFactory)
    {
        NetworkUtils::Initialize();
        GlobalContext::GetInstance().Initialize();
        GSessionManager->SetSessionFactory(sessionFactory);

        {
            _networkDispatcher = std::make_shared<EpollDispatcher>();
            auto epollDispatcher = std::static_pointer_cast<EpollDispatcher>(_networkDispatcher);
            
            if(epollDispatcher)
            {
                epollDispatcher->Initialize();
            }
        }
    }

    Server::Server(std::function<std::shared_ptr<Session>()> sessionFactory)
        : INetworkCore(sessionFactory), _acceptor(std::make_shared<Acceptor>())
    {

    }

    Server::~Server()
    {
        
    }

    bool Server::Start(uint16 port)
    {
        if(_acceptor == nullptr)
            return false;

        if(_networkDispatcher == nullptr)
            return false;

        _acceptor->_networkDispatcher = _networkDispatcher;
        if(_acceptor->Start(port) == false)
        {
            return false;
        }
        
        _port = port;
        _isRunning.store(true, std::memory_order_release);
            
        return true;
    }

    void Server::Stop()
    {
        if(_isRunning.load(std::memory_order_acquire) == false)
            return;
        
        std::cout << "Server Stopping..." << std::endl;

        if(_acceptor)
            _acceptor->Stop();

        INetworkCore::Stop();
    }
    
    Client::Client(std::function<std::shared_ptr<Session>()> sessionFactory)
        : INetworkCore(sessionFactory)
    {

    }

    Client::~Client()
    {

    }

    bool Client::Connect(NetworkAddress& targetAddress, int32 connectionCount)
    {
        std::vector<uint64> connectedSessions;  

        for (int i = 0; i < connectionCount; ++i)
        {
            auto session = GSessionManager->CreateSession();
     
            GSessionManager->AddSession(session);
            connectedSessions.push_back(session->GetSessionId());

            session->_networkDispatcher = _networkDispatcher;

            if (session->Connect(targetAddress) == false)
            {
                // 1) 이번 세션은 즉시 파기(정상 진입 전)
                GSessionManager->AbortSession(session->GetSessionId());

                // 2) 이미 만들어진 다른 세션들 정리
                for (auto id : connectedSessions)
                {
                    if (id == session->GetSessionId()) 
                        continue;

                    auto epollDispatcher = std::static_pointer_cast<EpollDispatcher>(_networkDispatcher);
                    epollDispatcher->PostRemoveSessionEvent(id);
                }

                return false;
            }
        }

        _isRunning.store(true, std::memory_order_release);
        return true;
    }

    void Client::Stop()
    {
        if(_isRunning.load(std::memory_order_acquire) == false)
            return;

        std::cout << "Client Stopping..." << std::endl;

        INetworkCore::Stop();
    }
    
}