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
		virtual void update(MILLISECONDS deltaTime) {}

	protected:
		static DWORD WINAPI ThreadProc(LPVOID lpParam);

		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);
		virtual void onThreadRun();
		virtual void onThreadExit() {}

		bool isStopEventSignaled(uint32 timeOut = 10) const;
		bool isRunning(uint32 timeOut = 50) const;


		HANDLE m_hThread;
		HANDLE m_hStopEvent;
		uint32  m_ThreadPriority;
	};
}