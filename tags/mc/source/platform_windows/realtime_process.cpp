#include "platform_windows.h"
#include "realtime_process.h"

namespace Pegas
{
	RealtimeProcess::RealtimeProcess(int32 threadPriority)
		:m_hThread(NULL), m_hStopEvent(NULL), m_ThreadPriority(threadPriority)
	{
		m_hStopEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL); 
	}

	RealtimeProcess::~RealtimeProcess()
	{
		if(m_hStopEvent != NULL)
		{
			::CloseHandle(m_hStopEvent);
		}

		if(m_hThread != NULL)
		{
			::CloseHandle(m_hThread);
		}
	}

	void RealtimeProcess::start(ProcessHandle myHandle, ProcessManagerPtr owner)
	{
		Process::start(myHandle, owner);

		m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, 0);

		assert(m_hThread && "Thread not created");
		if(m_hThread == NULL)
		{
			return;
		}
	}

	void RealtimeProcess::suspend()
	{
		assert(m_hThread && "Thread not created");
		if(m_hThread)
		{
			::SuspendThread(m_hThread);
		}

		Process::suspend();
	}

	void RealtimeProcess::resume()
	{
		assert(m_hThread && "Thread not created");
		if(m_hThread)
		{
			::ResumeThread(m_hThread);
		}

		Process::resume();
	}

	void RealtimeProcess::terminate()
	{
		//first check if the thread is running
		if(isRunning() && m_hStopEvent != NULL)
		{
			//trigger the event to signal the thread to come to an end
			if(::SetEvent(m_hStopEvent) == TRUE)
			{
				//give control to other threads
				::Sleep(0);
				//wait until thread has stopped
				::WaitForSingleObject(m_hThread, INFINITE);

				m_hThread = NULL;
			}
		}	

		Process::terminate();
	}

	bool RealtimeProcess::isStopEventSignaled(uint32 timeOut) const
	{
		//check if the stop-event object is signaled using the
		//passed-in timeout value
		if (::WaitForSingleObject(m_hStopEvent, timeOut) == WAIT_OBJECT_0)
		{
			return true;
		}

		return false;
	}

	bool RealtimeProcess::isRunning(uint32 timeOut) const
	{
		//INFO: thread-handle state is set to nonsignaled while the thread is running
		//and set to signaled when the thread terminates
		if (::WaitForSingleObject(m_hThread, timeOut/*ms*/) == WAIT_TIMEOUT)
		{
			return true; //the wait timed out -> thread handle is not signaled -> thread is running
		}

		return false;
	}

	void RealtimeProcess::onThreadRun()
	{
		MILLISECONDS previousTime = OSUtils::getInstance().getCurrentTime();
		while(!isStopEventSignaled())
		{
			MILLISECONDS currentTime = OSUtils::getInstance().getCurrentTime();
			MILLISECONDS deltaTime = previousTime - currentTime;
			previousTime = currentTime;

			update(deltaTime);
		}


	}

	DWORD WINAPI RealtimeProcess::ThreadProc(LPVOID lpParam)
	{
		RealtimeProcess* rtp = static_cast<RealtimeProcess*>(lpParam);
		
		rtp->onThreadRun();

		return TRUE;
	}
}