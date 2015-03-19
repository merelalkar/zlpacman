#pragma once

namespace Pegas
{
	class CriticalSection
	{
	public:
		CriticalSection();
		~CriticalSection();

		void lock();
		void unlock();
	
	private:
		mutable CRITICAL_SECTION m_cs;

		CriticalSection(const CriticalSection& cs);
		CriticalSection& operator=(const CriticalSection& cs);
	};

	class ScopedCriticalSection
	{
	public:
		ScopedCriticalSection(CriticalSection& csResource);
		~ScopedCriticalSection();
	private:
		CriticalSection& m_csResource;

		ScopedCriticalSection(const ScopedCriticalSection& scs);
		ScopedCriticalSection& operator=(const ScopedCriticalSection& scs);
	};
}