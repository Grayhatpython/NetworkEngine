#include "Pch.hpp"
#include "NetworkCore.hpp"
#include "NetworkUtils.hpp"
#include "SessionManager.hpp"
#include "NetworkDispatcher.hpp"

namespace servercore
{
    INetworkCore::INetworkCore(std::function<std::shared_ptr<Session>()> sessionFactory)
        : _sessionManager(std::make_shared<SessionManager>()), _networkDispatcher(std::make_shared<EpollDispatcher>())
    {
        NetworkUtils::Initialize();

        _sessionManager->SetSessionFactory(sessionFactory);
    }

    INetworkCore::~INetworkCore()
    {

    }

    Server::Server(std::function<std::shared_ptr<Session>()> sessionFactory)
        : INetworkCore(sessionFactory)
    {

    }

    Server::~Server()
    {

    }

    Client::Client(std::function<std::shared_ptr<Session>()> sessionFactory)
        : INetworkCore(sessionFactory)
    {

    }

    Client::~Client()
    {

    }
}