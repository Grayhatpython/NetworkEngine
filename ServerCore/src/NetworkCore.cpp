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
        : _globalContext(std::make_unique<GlobalContext>())
    {
        NetworkUtils::Initialize();
        _globalContext->Initialize();

        GSessionManager->SetSessionFactory(sessionFactory);
    }

    INetworkCore::~INetworkCore()
    {

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

        if(_globalContext == nullptr)
            return false;

        _acceptor->SetNetworkDispatcher(_networkDispatcher);
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
            assert(seession);

            GSessionManager->AddSession(seession);

            if (seession->Connect(targetAddress) == false) 
            {
                GSessionManager->RemoveSession(seession);
                return false;
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