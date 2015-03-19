#ifndef PEGAS_PLATFORM_WINMM_SOUND_MANAGER_H
#define PEGAS_PLATFORM_WINMM_SOUND_MANAGER_H
#pragma once

namespace Pegas
{
	class WinMMSoundManager: public SoundManager
	{
	public:
		WinMMSoundManager() {};
		virtual ~WinMMSoundManager() {};
		
		virtual void playSound(RESOURCEID soundID);
	private:
		WinMMSoundManager(const WinMMSoundManager& src);
		WinMMSoundManager& operator=(const WinMMSoundManager& src);
	};
}

#endif