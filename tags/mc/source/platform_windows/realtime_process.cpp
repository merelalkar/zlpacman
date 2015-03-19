#include "platform_windows.h"
#include "realtime_process.h"

namespace Pegas
{
	RealtimeProcess::RealtimeProcess(int32 threadPriority)
		:m_hThread(NULL), m_threadId(NULL), m_threadPriority(threadPriority);
	{

	}

	RealtimeProcess::~RealtimeProcess()
	{
		
	}

	void RealtimeProcess::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		m_hThread = ::CreateThread(NULL, 0, ThreadProc, 
			this, CREATE_SUSPENDED, &m_threadId);

		assert(m_hThread && "Thread not created");
		if(m_hThread == NULL)
		{
			return;
		}
	}

	void RealtimeProcess::suspend()
	{
		Process::suspend();

		assert(m_hThread && "Thread not created");
		if(m_hThread)
		{
			::SuspendThread(m_hThread);
		}
	}

	void RealtimeProcess::resume()
	{
		Process::resume();

		assert(m_hThread && "Thread not created");
		if(m_hThread)
		{
			::ResumeThread(m_hThread);
		}
	}

	void RealtimeProcess::terminate()
	{
		if(m_hThread != NULL)
		{
			::CloseHandle(m_hThread);
		}

		Process::terminate();
	}

	void RealtimeProcess::_threadProc()
	{

	}

	DWORD WINAPI RealtimeProcess::ThreadProc(LPVOID lpParam)
	{
		RealtimeProcess* rtp = static_cast<RealtimeProcess*>(lpParam);
		
		rtp->_threadProc();

		return TRUE;
	}
}