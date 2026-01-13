#pragma once

namespace servercore
{
	class MemoryPool;
	class ThreadManager;
	class SessionManager;
	class GlobalContext
	{
	public:
		GlobalContext(const GlobalContext&) = delete;
		GlobalContext& operator=(const GlobalContext&) = delete;
	
	private:
		GlobalContext() = default;
		~GlobalContext();

	public:
		static GlobalContext& GetInstance()
		{
			static GlobalContext instance;
			return instance;
		}
	
		bool Initialize();
		void Clear();

	public:
		MemoryPool* 		GetMemoryPool() { return _memoryPool;}
		ThreadManager* 		GetThreadPool() { return _threadManager;}
		SessionManager*		GetSessionManager() { return _sessionManager; }


	private:
		MemoryPool* 		_memoryPool = nullptr;
		ThreadManager* 		_threadManager = nullptr;
		SessionManager* 	_sessionManager= nullptr;
	};

#define GMemoryPool		GlobalContext::GetInstance().GetMemoryPool()
#define GThreadManager	GlobalContext::GetInstance().GetThreadPool()
#define GSessionManager	GlobalContext::GetInstance().GetSessionManager()
}

