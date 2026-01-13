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

    Server::Server(std::function<std::shared_ptr<Session>()> sessionFactory)
        : INetworkCore(sessionFactory), _acceptor(std::make_shared<Acceptor>())
    {

    }

    Server::~Server()
    {

    }

    bool Server::Start(uint16 port)
    {

        return true;
    }

    Client::Client(std::function<std::shared_ptr<Session>()> sessionFactory)
        : INetworkCore(sessionFactory)
    {

    }

    Client::~Client()
    {

    }
}