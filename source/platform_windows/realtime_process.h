#pragma once

namespace Pegas
{
	class RealtimeProcess: public Process
	{
	public:
		RealtimeProcess(int32 threadPriority = THREAD_PRIORITY_NORMAL);
		virtual ~RealtimeProcess();

		virtual void suspend();
		virtual void resume();
		virtual void terminate();

		static DWORD WINAPI ThreadProc(LPVOID lpParam);
	
	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

	private:
		//вызывается из ThreadProc
		void _threadProc();

		HANDLE m_hThread;
		DWORD  m_threadId;
		int32  m_threadPriority;
	};
}