#include "platform_windows.h"
#include "critical_section.h"

namespace Pegas
{
	/**************************************************************
		
	***************************************************************/
	CriticalSection::CriticalSection()
	{
		::InitializeCriticalSection(&m_cs);
	}

	CriticalSection::~CriticalSection()
	{
		::DeleteCriticalSection(&m_cs);
	}

	void CriticalSection::lock()
	{
		::EnterCriticalSection(&m_cs);
	}

	void CriticalSection::unlock()
	{
		::LeaveCriticalSection(&m_cs);
	}

	/********************************************************************

	*********************************************************************/
	ScopedCriticalSection::ScopedCriticalSection(CriticalSection& csResource)
		:m_csResource(csResource)
	{
		m_csResource.lock();
	}

	ScopedCriticalSection::~ScopedCriticalSection()
	{
		m_csResource.unlock();
	}
}