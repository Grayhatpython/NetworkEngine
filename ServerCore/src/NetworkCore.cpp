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
        GlobalContext::GetInstance().Clear();
    }

    void INetworkCore::Stop()
    {

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
        //  TODO 

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
        for (auto i = 0; i < connectionCount; i++)
        {
            auto seession = GSessionManager->CreateSession();

            if(seession)
            {
                GSessionManager->AddSession(seession);
                seession->_networkDispatcher = _networkDispatcher;

                if (seession->Connect(targetAddress) == false) 
                {
                    GSessionManager->RemoveSession(seession);
                    return false;
                }
            }
        }
        
        _isRunning.store(true, std::memory_order_release);
        return true;
    }

    void Client::Stop()
    {
        //  TODO 
        
        INetworkCore::Stop();
    }
    
}