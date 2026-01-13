#pragma once    

namespace servercore
{
    class Session;
    class Acceptor;
    class GlobalContext;
    class SessionManager;
    class INetworkDispatcher;

    class INetworkCore : public std::enable_shared_from_this<INetworkCore>
    {
    public:
        INetworkCore(std::function<std::shared_ptr<Session>()> sessionFactory);
        virtual ~INetworkCore();

    public:

    protected:
        std::unique_ptr<GlobalContext>                  _globalContext;
        std::shared_ptr<INetworkDispatcher>             _networkDispatcher;

        std::mutex                                      _mutex;
        std::condition_variable                         _cv;
        std::atomic<bool>                               _isRunning{false};
        std::vector<std::thread>                        _workerThreads;
        int32                                           _workerThreadCount = 0;

    };

    class Server : public INetworkCore
    {
    public:
       Server(std::function<std::shared_ptr<Session>()> sessionFactory);
       virtual ~Server() override;

    public:
        bool Start(uint16 port);

    private:
        NetworkAddress						_listenNetworkAddress;
		std::shared_ptr<Acceptor>			_acceptor;
		uint16								_port = 0;
    };

    class Client : public INetworkCore
    {
    public:
       Client(std::function<std::shared_ptr<Session>()> sessionFactory);
       virtual ~Client() override;

    public:
        bool Connect(NetworkAddress& targetAddress);

    private:

    };
}